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

#include "buf_surface_impl_system.h"

#include <cstdlib>  // for malloc/free
#include <cstring>  // for memset
#include <string>

#include "glog/logging.h"

#include "buf_surface_utils.h"
#include "nvis/infer_server.h"
#include "common/utils.hpp"

namespace infer_server {
    int MemAllocatorSystem::Create(BufSurfaceCreateParams *params) {
        create_params_ = *params;

        if (create_params_.batch_size == 0) {
            create_params_.batch_size = 1;
        }

        uint32_t alignment_w = 16;
        uint32_t alignment_h = 2;
        if (params->force_align_1) {
            alignment_w = alignment_h = 1;
        }

        block_size_ = params->size;

        if (!block_size_) {
            int ret = GetColorFormatDataSize(params->color_format, params->width, params->height, alignment_w, alignment_h
                , width_stride_, height_stride_, block_size_);
            if (ret != 0) return ret;
        }
        else {
            block_size_ = (block_size_ + alignment_w - 1) / alignment_w * alignment_w;
        }

        created_ = true;
        return 0;
    }

    int MemAllocatorSystem::Destroy() {
        created_ = false;
        return 0;
    }

    int MemAllocatorSystem::Alloc(BufSurface *surf) {
        void *addr = MallocHostMem(block_size_ * create_params_.batch_size);

        memset(surf, 0, sizeof(BufSurface));
        surf->mem_type = create_params_.mem_type;
        surf->opaque = nullptr;  // will be filled by MemPool
        surf->batch_size = create_params_.batch_size;
        surf->num_filled = create_params_.batch_size;
        surf->device_id = create_params_.device_id;
        surf->surface_list = reinterpret_cast<BufSurfaceParams *>(malloc(sizeof(BufSurfaceParams) * surf->batch_size));
        memset(surf->surface_list, 0, sizeof(BufSurfaceParams) * surf->batch_size);
        uint8_t *addr8 = reinterpret_cast<uint8_t *>(addr);
        for (uint32_t i = 0; i < surf->batch_size; i++) {
            surf->surface_list[i].color_format = create_params_.color_format;
            surf->surface_list[i].data_ptr = addr8;
            addr8 += block_size_;
            surf->surface_list[i].width = create_params_.width;
            surf->surface_list[i].height = create_params_.height;
            surf->surface_list[i].width_stride = width_stride_;
            surf->surface_list[i].height_stride = height_stride_;        
            surf->surface_list[i].data_size = block_size_;
        }
        return 0;
    }

    int MemAllocatorSystem::Free(BufSurface *surf) {
        void *addr = surf->surface_list[0].data_ptr;
        size_t dsize = surf->surface_list[0].data_size * surf->batch_size;
        FreeHostMem(addr);
        ::free(reinterpret_cast<void *>(surf->surface_list));
        return 0;
    }

}  // namespace 
