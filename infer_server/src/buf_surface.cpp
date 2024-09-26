/*************************************************************************
 * Copyright (C) [2022] by Cambricon, Inc. All rights reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a Copy of the License at
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

#include "buf_surface.h"

#include <algorithm>
#include <cstring>  // for memset
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "glog/logging.h"

#include "buf_surface_impl.h"
#include "nvis/infer_server.h"
#include "common/utils.hpp"

namespace infer_server {

    class BufSurfaceService {
    public:
        static BufSurfaceService &Instance() {
            static std::once_flag s_flag;
            std::call_once(s_flag, [&] { instance_.reset(new BufSurfaceService); });
            return *instance_;
        }
        ~BufSurfaceService() = default;
        int BufPoolCreate(void **pool, BufSurfaceCreateParams *params, uint32_t block_num) {
            if (pool && params && block_num) {
                MemPool *mempool = new MemPool();
                if (!mempool) {
                    LOG(ERROR) << "[InferServer] [BufSurfaceService] BufPoolCreate(): new memory pool failed";
                    return -1;
                }
                *pool = reinterpret_cast<void *>(mempool);
                if (mempool->Create(params, block_num) == 0) {
                    return 0;
                }
                delete mempool;
                LOG(ERROR) << "[InferServer] [BufSurfaceService] BufPoolCreate(): Create memory pool failed";
                return -1;
            }
            return -1;
        }
        int BufPoolDestroy(void *pool) {
            if (pool) {
                MemPool *mempool = reinterpret_cast<MemPool *>(pool);
                if (mempool) {
                    int ret = mempool->Destroy();
                    if (ret != 0) {
                        VLOG(3) << "[InferServer] [BufSurfaceService] BufPoolDestroy(): Destroy memory pool failed, ret = " << ret;
                        return ret;
                    }
                    delete mempool;
                }
                return 0;
            }
            LOG(ERROR) << "[InferServer] [BufSurfaceService] BufPoolDestroy(): Pool is not existed";
            return -1;
        }
        int CreateFromPool(BufSurface **surf, void *pool) {
            if (surf && pool) {
                BufSurface surface;
                MemPool *mempool = reinterpret_cast<MemPool *>(pool);
                if (mempool->Alloc(&surface) < 0) {
                    VLOG(4) << "[InferServer] [BufSurfaceService] CreateFromPool(): Create BufSurface from pool failed";
                    return -1;
                }
                *surf = AllocSurface();
                if (!(*surf)) {
                    mempool->Free(&surface);
                    LOG(WARNING) << "[InferServer] [BufSurfaceService] CreateFromPool(): Alloc BufSurface failed";
                    return -1;
                }
                *(*surf) = surface;
                return 0;
            }
            LOG(ERROR) << "[InferServer] [BufSurfaceService] CreateFromPool(): surf or pool is nullptr";
            return -1;
        }
        int Create(BufSurface **surf, BufSurfaceCreateParams *params) {
            if (surf && params) {
                if (CheckParams(params) < 0) {
                    LOG(ERROR) << "[InferServer] [BufSurfaceService] Create(): Parameters are invalid";
                    return -1;
                }
                BufSurface surface;
                if (CreateSurface(params, &surface) < 0) {
                    LOG(ERROR) << "[InferServer] [BufSurfaceService] Create(): Create BufSurface failed";
                    return -1;
                }
                *surf = AllocSurface();
                if (!(*surf)) {
                    DestroySurface(&surface);
                    LOG(ERROR) << "[InferServer] [BufSurfaceService] Create(): Alloc BufSurface failed";
                    return -1;
                }
                *(*surf) = surface;
                return 0;
            }
            LOG(ERROR) << "[InferServer] [BufSurfaceService] Create(): surf or params is nullptr";
            return -1;
        }

        int Destroy(BufSurface *surf) {
            if (!surf) {
                LOG(ERROR) << "[InferServer] [BufSurfaceService] Destroy(): surf is nullptr";
                return -1;
            }

            if (surf->opaque) {
                MemPool *mempool = reinterpret_cast<MemPool *>(surf->opaque);
                int ret = mempool->Free(surf);
                FreeSurface(surf);
                if (ret) {
                    LOG(ERROR) << "[InferServer] [BufSurfaceService] Destroy(): Free BufSurface back to memory pool failed";
                }
                return ret;
            }

            // created by BufSurfaceCreate()
            int ret = DestroySurface(surf);
            FreeSurface(surf);
            if (ret) {
                LOG(ERROR) << "[InferServer] [BufSurfaceService] Destroy(): Destroy BufSurface failed";
            }
            return ret;
        }

        int Memset(BufSurface *surf, int index, uint8_t value) {
            if (!surf) {
                LOG(ERROR) << "[InferServer] [BufSurfaceService] Memset(): BufSurface is nullptr";
                return -1;
            }
            if (index < -1 || index >= static_cast<int>(surf->batch_size)) {
                LOG(ERROR) << "[InferServer] [BufSurfaceService] Memset(): batch index is invalid";
                return -1;
            }

            for (uint32_t i = 0; i < surf->batch_size; i++) {
                if (index >= 0 && i != static_cast<uint32_t>(index)) continue;
                CUDA_SAFECALL(cudaMemset(surf->surface_list[i].data_ptr, value, surf->surface_list[i].data_size), "[InferServer] [BufSurfaceService] Memset(): failed", -1);
            }
            return 0;
        }

        int Copy(BufSurface *src_surf, BufSurface *dst_surf) {
            if (!src_surf || !dst_surf) {
                LOG(ERROR) << "[InferServer] [BufSurfaceService] Copy(): src or dst BufSurface is nullptr";
                return -1;
            }
            // check parameters, must be the same size
            if (src_surf->batch_size != dst_surf->batch_size) {
                LOG(ERROR) << "[InferServer] [BufSurfaceService] Copy(): src and dst BufSurface has different batch size";
                return -1;
            }

            dst_surf->pts = src_surf->pts;
            bool src_host = (src_surf->mem_type == BUF_MEMORY_HOST);

            bool dst_host = (dst_surf->mem_type == BUF_MEMORY_HOST);

            if ((!dst_host && !src_host) && (src_surf->device_id != dst_surf->device_id)) {
                LOG(ERROR) << "[InferServer] [BufSurfaceService] Copy(): src and dst BufSurface is on different device";
                return -1;
            }

            for (size_t i = 0; i < src_surf->batch_size; ++i) {
                CUDA_SAFECALL(MemcpyHD(dst_surf->surface_list[i].data_ptr, dst_surf->mem_type
                    , src_surf->surface_list[i].data_ptr, src_surf->mem_type, dst_surf->surface_list[i].data_size),
                    "[InferServer] [BufSurfaceService] Copy(): failed", -1);
            }

            return 0;
        }

    private:
        BufSurfaceService(const BufSurfaceService &) = delete;
        BufSurfaceService(BufSurfaceService &&) = delete;
        BufSurfaceService &operator=(const BufSurfaceService &) = delete;
        BufSurfaceService &operator=(BufSurfaceService &&) = delete;
        BufSurfaceService() = default;

    private:
        std::mutex mutex_;
        bool initialized_ = false;
        std::queue<BufSurface *> surfpool_;
        BufSurface *start_ = nullptr, *end_ = nullptr;
        static const int k_surfs_num_ = 256 * 1024;

    private:
        void CreateSurfsPool() {
            if (initialized_) return;
            start_ = reinterpret_cast<BufSurface *>(malloc(sizeof(BufSurface) * k_surfs_num_));
            if (!start_) {
                LOG(ERROR) << "[InferServer] [BufSurfaceService] CreateSurfsPool(): Create BufSurface pointers failed";
                return;
            }
            end_ = &start_[k_surfs_num_ - 1];
            for (int i = 0; i < k_surfs_num_; i++) surfpool_.push(&start_[i]);
            initialized_ = true;
        }

        BufSurface *AllocSurface() {
            std::unique_lock<std::mutex> lk(mutex_);
            if (!initialized_) CreateSurfsPool();
            if (!surfpool_.empty()) {
                BufSurface *res = surfpool_.front();
                surfpool_.pop();
                return res;
            }
            return reinterpret_cast<BufSurface *>(malloc(sizeof(BufSurface)));
        }

        void FreeSurface(BufSurface *surf) {
            std::unique_lock<std::mutex> lk(mutex_);
            if (surf >= start_ && surf <= end_) {
                surfpool_.push(surf);
                return;
            }
            ::free(surf);
        }

    private:
        static std::unique_ptr<BufSurfaceService> instance_;
    };

    std::unique_ptr<BufSurfaceService> BufSurfaceService::instance_;

}  // namespace 

extern "C" {
    int BufPoolCreate(void **pool, BufSurfaceCreateParams *params, uint32_t block_num) {
        return infer_server::BufSurfaceService::Instance().BufPoolCreate(pool, params, block_num);
    }

    int BufPoolDestroy(void *pool) { return infer_server::BufSurfaceService::Instance().BufPoolDestroy(pool); }

    int BufSurfaceCreateFromPool(BufSurface **surf, void *pool) {
        return infer_server::BufSurfaceService::Instance().CreateFromPool(surf, pool);
    }

    int BufSurfaceCreate(BufSurface **surf, BufSurfaceCreateParams *params) {
        return infer_server::BufSurfaceService::Instance().Create(surf, params);
    }

    int BufSurfaceDestroy(BufSurface *surf) { return infer_server::BufSurfaceService::Instance().Destroy(surf); }

    int BufSurfaceMemSet(BufSurface *surf, int index, uint8_t value) {
        return infer_server::BufSurfaceService::Instance().Memset(surf, index, value);
    }

    int BufSurfaceCopy(BufSurface *src_surf, BufSurface *dst_surf) {
        return infer_server::BufSurfaceService::Instance().Copy(src_surf, dst_surf);
    }

};  // extern "C"
