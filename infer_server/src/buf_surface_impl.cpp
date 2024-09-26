/*************************************************************************
 * Copyright (C) [2022] by Cambricon, Inc. All rights reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *************************************************************************/

#include "buf_surface_impl.h"

#include <string>
#include <thread>

#include "glog/logging.h"

#include "buf_surface_impl_device.h"
#include "buf_surface_impl_system.h"

namespace infer_server {

    int MemPool::Create(BufSurfaceCreateParams *params, uint32_t block_num) {
        std::unique_lock<std::mutex> lk(mutex_);
        if (created_) {
            LOG(ERROR) << "[InferServer] [MemPool] Create(): Pool has been created";
            return -1;
        }

        if (CheckParams(params) < 0) {
            LOG(ERROR) << "[InferServer] [MemPool] Create(): Parameters are invalid";
            return -1;
        }

        allocator_ = CreateMemAllocator(params->mem_type);
        if (!allocator_) {
            LOG(ERROR) << "[InferServer] [MemPool] Create(): Create memory allocator pointer failed";
            return -1;
        }

        if (allocator_->Create(params) < 0) {
            LOG(ERROR) << "[InferServer] [MemPool] Create(): Memory allocator initialize resources failed";
            return -1;
        }

        // cache the blocks
        for (uint32_t i = 0; i < block_num; i++) {
            BufSurface surf;
            if (allocator_->Alloc(&surf) < 0) {
                LOG(ERROR) << "[InferServer] [MemPool] Create(): Memory allocator alloc BufSurface failed";
                return -1;
            }
            surf.opaque = reinterpret_cast<void *>(this);
            cache_.push(surf);
        }

        alloc_count_ = 0;
        created_ = true;
        return 0;
    }

    int MemPool::Destroy() {
        std::unique_lock<std::mutex> lk(mutex_);
        if (!created_) {
            LOG(ERROR) << "[InferServer] [MemPool] Destroy(): Memory pool is not created";
            return -1;
        }

        while (alloc_count_) {
            lk.unlock();
            std::this_thread::yield();
            lk.lock();
        }
        while (!cache_.empty()) {
            auto surf = cache_.front();
            cache_.pop();
            allocator_->Free(&surf);
        }

        // FIXME
        if (allocator_->Destroy() < 0) {
            VLOG(3) << "[InferServer] [MemPool] Destroy(): Destroy memory allocator failed";
            return -1;
        }
        delete allocator_, allocator_ = nullptr;

        alloc_count_ = 0;
        created_ = false;
        return 0;
    }

    int MemPool::Alloc(BufSurface *surf) {
        std::unique_lock<std::mutex> lk(mutex_);
        if (!created_) {
            LOG(ERROR) << "[InferServer] [MemPool] Alloc(): Memory pool is not created";
            return -1;
        }

        if (cache_.empty()) {
            VLOG(4) << "[InferServer] [MemPool] Alloc(): Memory cache is empty";
            return -1;
        }

        *surf = cache_.front();
        cache_.pop();

        ++alloc_count_;
        return 0;
    }

    int MemPool::Free(BufSurface *surf) {
        std::unique_lock<std::mutex> lk(mutex_);
        if (!created_) {
            LOG(ERROR) << "[InferServer] [MemPool] Free(): Memory pool is not created";
            return -1;
        }

        cache_.push(*surf);
        --alloc_count_;
        return 0;
    }

    //
    IMemAllcator *CreateMemAllocator(BufSurfaceMemType mem_type) {
        if (mem_type == BUF_MEMORY_HOST) {
            return new MemAllocatorSystem();
        }
        else if (mem_type == BUF_MEMORY_MAP) {
            return new MemAllocatorDevice();
        }
        LOG(ERROR) << "[InferServer] CreateMemAllocator(): Unsupported memory type: " << mem_type;
        return nullptr;
    }

    // for non-pool case
    int CreateSurface(BufSurfaceCreateParams *params, BufSurface *surf) {
        if (CheckParams(params) < 0) {
            LOG(ERROR) << "[InferServer] CreateSurface(): Parameters are invalid";
            return -1;
        }

        if (params->mem_type == BUF_MEMORY_HOST) {
            MemAllocatorSystem allocator;
            if (allocator.Create(params) < 0) {
                LOG(ERROR) << "[InferServer] CreateSurface(): Memory allocator initialize resources failed. mem_type = "
                    << params->mem_type;
                return -1;
            }
            if (allocator.Alloc(surf) < 0) {
                LOG(ERROR) << "[InferServer] CreateSurface(): Memory allocator create BufSurface failed. mem_type = "
                    << params->mem_type;
                return -1;
            }
            return 0;
        }

        if (params->mem_type == BUF_MEMORY_MAP) {
            MemAllocatorDevice allocator;
            if (allocator.Create(params) < 0) {
                LOG(ERROR) << "[InferServer] CreateSurface(): Memory allocator initialize resources failed. mem_type = "
                    << params->mem_type;
                return -1;
            }
            if (allocator.Alloc(surf) < 0) {
                LOG(ERROR) << "[InferServer] CreateSurface(): Memory allocator create BufSurface failed. mem_type = "
                    << params->mem_type;
                return -1;
            }
            return 0;
        }

        LOG(ERROR) << "[InferServer] CreateSurface(): Unsupported memory type: " << params->mem_type;
        return -1;
    }

    int DestroySurface(BufSurface *surf) {
        if (surf->mem_type == BUF_MEMORY_HOST) {
            MemAllocatorSystem allocator;
            if (allocator.Free(surf) < 0) {
                LOG(ERROR) << "[InferServer] DestroySurface(): Memory allocator free BufSurface failed. mem_type = "
                    << surf->mem_type;
                return -1;
            }
            if (allocator.Destroy() < 0) {
                LOG(ERROR) << "[InferServer] DestroySurface(): Memory allocator destroy BufSurface failed. mem_type = "
                    << surf->mem_type;
                return -1;
            }
            return 0;
        }

        if (surf->mem_type == BUF_MEMORY_MAP) {
            MemAllocatorDevice allocator;
            if (allocator.Free(surf) < 0) {
                LOG(ERROR) << "[InferServer] DestroySurface(): Memory allocator destroy BufSurface failed. mem_type = "
                    << surf->mem_type;
                return -1;
            }
            return 0;
        }

        LOG(ERROR) << "[InferServer] DestroySurface(): Unsupported memory type: " << surf->mem_type;
        return -1;
    }

}  // namespace 
