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

#include "buf_surface_utils.h"
#include "common/utils.hpp"

#include <cstring>  // for memset
#include <map>
#include <mutex>
#include <string>

#include "glog/logging.h"

namespace infer_server {

    int GetColorFormatDataSize(BufSurfaceColorFormat fmt, uint32_t width, uint32_t height, uint32_t align_size_w, uint32_t align_size_h
        , uint32_t& width_stride, uint32_t& height_stride, uint32_t& data_size) {
        switch (fmt) {
        case BUF_COLOR_FORMAT_NV12:
        case BUF_COLOR_FORMAT_NV21:
            width_stride = ALIGN_UP(width, align_size_w);
            height_stride = ALIGN_UP(height, align_size_h);
            data_size = width_stride* height_stride * 3 / 2;
            return 0;
        case BUF_COLOR_FORMAT_RGB:
        case BUF_COLOR_FORMAT_BGR:
            width_stride = ALIGN_UP(width, align_size_w) * 3;
            height_stride = ALIGN_UP(height, align_size_h);
            data_size = width_stride * height_stride;
            return 0;
        case BUF_COLOR_FORMAT_INVALID:
        default: {
            LOG(ERROR) << "[InferServer] GetColorFormatInfo(): Unsupported color format: " << fmt;
            return -1;
        }
        }  // switch
    }

    int CheckParams(BufSurfaceCreateParams *params) {
        if (params->batch_size == 0) {
            LOG(ERROR) << "[InferServer] CheckParams(): Invalid batch_size = " << params->batch_size;
            return -1;
        }

        if (params->mem_type != BUF_MEMORY_HOST) {
            if (params->mem_type < BUF_MEMORY_HOST) {
                LOG(ERROR) << "[InferServer] CheckParams(): Unsupported memory type: " << params->mem_type;
                return -1;
            }

            if (params->mem_type != BUF_MEMORY_MAP) {
                LOG(ERROR) << "[InferServer] CheckParams(): For not support unified address, unsupported memory type: "
                    << params->mem_type;
                return -1;
            }

            return 0;
        }

        if (params->color_format < BUF_COLOR_FORMAT_INVALID || params->color_format >= BUF_COLOR_FORMAT_LAST) {
            LOG(ERROR) << "[InferServer] CheckParams(): Unknown color format: " << params->color_format;
            return -1;
        }

        if (params->width * params->height == 0 && params->size == 0) {
            LOG(ERROR) << "[InferServer] CheckParams(): Invalid width, height or size. w = " << params->width
                << ", h = " << params->height << ", size = " << params->size;
            return -1;
        }

        return 0;
    }

}  // namespace 
