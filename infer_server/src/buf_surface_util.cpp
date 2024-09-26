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
#include "buf_surface_util.hpp"
#include "nvis/infer_server.h"

#include <unistd.h>

#include <algorithm>
#include <memory>
#include <thread>

#include "common/utils.hpp"
#include "glog/logging.h"

namespace infer_server {
    //
    // BufSurfaceWrapper
    //
    BufSurface *BufSurfaceWrapper::GetBufSurface() const {
        std::unique_lock<std::mutex> lk(mutex_);
        return surf_;
    }

    BufSurface *BufSurfaceWrapper::BufSurfaceChown() {
        std::unique_lock<std::mutex> lk(mutex_);
        BufSurface *surf = surf_;
        surf_ = nullptr;
        return surf;
    }

    BufSurfaceParams *BufSurfaceWrapper::GetSurfaceParams(uint32_t batch_idx) const {
        std::unique_lock<std::mutex> lk(mutex_);
        return &surf_->surface_list[batch_idx];
    }

    uint32_t BufSurfaceWrapper::GetNumFilled() const {
        std::unique_lock<std::mutex> lk(mutex_);
        return surf_->num_filled;
    }

    BufSurfaceColorFormat BufSurfaceWrapper::GetColorFormat(uint32_t batch_idx) const {
        std::unique_lock<std::mutex> lk(mutex_);
        return GetSurfaceParamsPriv(batch_idx)->color_format;
    }

    uint32_t BufSurfaceWrapper::GetWidth(uint32_t batch_idx) const {
        std::unique_lock<std::mutex> lk(mutex_);
        return GetSurfaceParamsPriv(batch_idx)->width;
    }

    uint32_t BufSurfaceWrapper::GetHeight(uint32_t batch_idx) const {
        std::unique_lock<std::mutex> lk(mutex_);
        return GetSurfaceParamsPriv(batch_idx)->height;
    }

    uint32_t BufSurfaceWrapper::GetWidthStride(uint32_t batch_idx) const {
        std::unique_lock<std::mutex> lk(mutex_);
        return GetSurfaceParamsPriv(batch_idx)->width_stride;
    }

    uint32_t BufSurfaceWrapper::GetHeightStride(uint32_t batch_idx) const {
        std::unique_lock<std::mutex> lk(mutex_);
        return GetSurfaceParamsPriv(batch_idx)->height_stride;
    }

    int BufSurfaceWrapper::GetDeviceId() const {
        std::unique_lock<std::mutex> lk(mutex_);
        return surf_->device_id;
    }

    BufSurfaceMemType BufSurfaceWrapper::GetMemType() const {
        std::unique_lock<std::mutex> lk(mutex_);
        return surf_->mem_type;
    }

    void *BufSurfaceWrapper::GetData(uint32_t batch_idx) {
        std::unique_lock<std::mutex> lk(mutex_);
        BufSurfaceParams *params = GetSurfaceParamsPriv(batch_idx);
        return params->data_ptr;
    }

    uint32_t BufSurfaceWrapper::GetSize(uint32_t batch_idx) {
        std::unique_lock<std::mutex> lk(mutex_);
        BufSurfaceParams *params = GetSurfaceParamsPriv(batch_idx);
        return params->data_size;
    }

    uint64_t BufSurfaceWrapper::GetPts() const {
        std::unique_lock<std::mutex> lk(mutex_);
        if (surf_) {
            return surf_->pts;
        }
        else {
            return pts_;
        }
    }

    void BufSurfaceWrapper::SetPts(uint64_t pts) {
        std::unique_lock<std::mutex> lk(mutex_);
        if (surf_) {
            surf_->pts = pts;
        }
        else {
            pts_ = pts;
        }
    }
    void *BufSurfaceWrapper::GetHostData(uint32_t batch_idx) {
        std::unique_lock<std::mutex> lk(mutex_);
        BufSurfaceParams *params = GetSurfaceParamsPriv(batch_idx);
        void *addr = params->data_ptr;

        if (addr) {
            return addr;
        }

        LOG(ERROR) << "[InferServer] [BufSurfaceWrapper] GetHostData(): Unsupported memory type";
        return nullptr;
    }

    void BufSurfaceWrapper::SyncHostToDevice(uint32_t batch_idx) {
        SetCurrentDevice(GetDeviceId());

        std::unique_lock<std::mutex> lk(mutex_);

        if (surf_->mem_type == BUF_MEMORY_MAP) {
            if (batch_idx >= 0 && batch_idx < 128 && host_data_[batch_idx]) {
                CALL_CUDA_FUNC(MemcpyHD(surf_->surface_list[batch_idx].data_ptr, BUF_MEMORY_MAP
                    , host_data_[batch_idx].get(), surf_->mem_type, GetSurfaceParamsPriv(batch_idx)->data_size)
                    , "[BufSurfaceWrapper] SyncHostToDevice(): copy data H2D failed, batch_idx = " + std::to_string(batch_idx));

                return;
            }

            if (batch_idx == (uint32_t)(-1)) {
                if (surf_->is_contiguous) {
                    if (!host_data_[0]) {
                        LOG(ERROR) << "[InferServer] [BufSurfaceWrapper] SyncHostToDevice(): Host data is null";
                        return;
                    }
                    size_t total_size = surf_->batch_size * GetSurfaceParamsPriv(0)->data_size;

                    CALL_CUDA_FUNC(MemcpyHD(surf_->surface_list[0].data_ptr, BUF_MEMORY_MAP
                        , host_data_[0].get(), surf_->mem_type, total_size)
                        , "[BufSurfaceWrapper] SyncHostToDevice(): data is contiguous, copy data H2D failed");
                }
                else {
                    if (surf_->batch_size >= 128) {
                        LOG(ERROR) << "[InferServer] [BufSurfaceWrapper] SyncHostToDevice: batch size should not be greater than 128,"
                            << " which is: " << surf_->batch_size;
                        return;
                    }
                    for (uint32_t i = 0; i < surf_->batch_size; i++) {
                        CALL_CUDA_FUNC(MemcpyHD(surf_->surface_list[i].data_ptr, BUF_MEMORY_MAP
                            , host_data_[i].get(), surf_->mem_type, surf_->surface_list[i].data_size)
                            , "[BufSurfaceWrapper] SyncHostToDevice(): copy data H2D failed, batch_idx = " + std::to_string(batch_idx));
                    }
                }
            }
            return;
        }
        LOG(ERROR) << "[InferServer] [BufSurfaceWrapper] SyncHostToDevice(): Unsupported memory type";
    }

    //
    // BufPool
    //
    int BufPool::CreatePool(BufSurfaceCreateParams *params, uint32_t block_count) {
        std::unique_lock<std::mutex> lk(mutex_);

        int ret = BufPoolCreate(&pool_, params, block_count);
        if (ret != 0) {
            LOG(ERROR) << "[InferServer] [BufPool] CreatePool(): Create BufSurface pool failed";
            return -1;
        }

        stopped_ = false;
        VLOG(2) << "[InferServer] [BufPool] CreatePool(): Done";
        return 0;
    }

    void BufPool::DestroyPool(int timeout_ms) {
        std::unique_lock<std::mutex> lk(mutex_);
        if (stopped_) {
            LOG(INFO) << "[InferServer] [BufPool] DestroyPool(): Pool has been stooped";
            return;
        }
        stopped_ = true;
        int count = timeout_ms + 1;
        int retry_cnt = 1;
        while (1) {
            if (pool_) {
                int ret;
                ret = BufPoolDestroy(pool_);
                if (ret == 0) {
                    return;
                }

                count -= retry_cnt;
                VLOG(3) << "[InferServer] [BufPool] DestroyPool(): retry, remaining times: " << count;
                if (count <= 0) {
                    LOG(ERROR) << "[InferServer] [BufPool] DestroyPool(): Maximum number of attempts reached: " << timeout_ms;
                    return;
                }

                lk.unlock();
                usleep(1000 * retry_cnt);
                retry_cnt = std::min(retry_cnt * 2, 10);
                lk.lock();
            }
            return;
        }
    }

    BufSurfWrapperPtr BufPool::GetBufSurfaceWrapper(int timeout_ms) {
        std::unique_lock<std::mutex> lk(mutex_);
        if (!pool_) {
            LOG(ERROR) << "[InferServer] [BufPool] GetBufSurfaceWrapper(): Pool is not created";
            return nullptr;
        }

        BufSurface *surf = nullptr;
        int count = timeout_ms + 1;
        int retry_cnt = 1;
        while (1) {
            if (stopped_) {
                // Destroy called, disable alloc-new-block
                LOG(ERROR) << "[InferServer] [BufPool] GetBufSurfaceWrapper(): Pool is stopped";
                return nullptr;
            }

            int ret = BufSurfaceCreateFromPool(&surf, pool_);
            if (ret == 0) {
                return std::make_shared<BufSurfaceWrapper>(surf);
            }
            count -= retry_cnt;
            VLOG(3) << "[InferServer] [BufPool] GetBufSurfaceWrapper(): retry, remaining times: " << count;
            if (count <= 0) {
                LOG(ERROR) << "[InferServer] [BufPool] GetBufSurfaceWrapper(): Maximum number of attempts reached: "
                    << timeout_ms;
                return nullptr;
            }

            lk.unlock();
            usleep(1000 * retry_cnt);
            retry_cnt = std::min(retry_cnt * 2, 10);
            lk.lock();
        }
        return nullptr;
    }

}  // namespace infer_server
