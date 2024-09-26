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

#ifndef COMMON_UTILS_HPP_
#define COMMON_UTILS_HPP_

#include <string>
#include <map>
#include <functional>
#include <glog/logging.h>

#include "buf_surface.h"

#include "cuda_runtime.h"

#include <nvcv/ImageFormat.hpp>

extern "C" {
#include <libavutil/pixfmt.h>
}

namespace infer_server {
    // defer(xxx) like golang's defer
#define DEFER _DEFER_ACTION_MAKE /* ([&](){ ... }); */

#define _DEFER_ACTION_MAKE auto \
        _DEFER_ACTION_VAR(_defer_action_line, __LINE__, _) = _DeferredActionCtor
#define _DEFER_ACTION_VAR(a, b, c) _DEFER_TOKEN_CONNECT(a, b, c)
#define _DEFER_TOKEN_CONNECT(a, b, c) a ## b ## c

    class _DeferredAction {
    private:
        _DeferredAction(_DeferredAction const&) = delete;
        _DeferredAction& operator=(_DeferredAction const&) = delete;

        std::function<void()> func_;

        template<typename T>
        friend _DeferredAction _DeferredActionCtor(T&& p);

        template<typename T>
        _DeferredAction(T&& p) : func_(std::forward<T>(p)) {}

    public:
        _DeferredAction(_DeferredAction&& other) :
            func_(other.func_) {
            other.func_ = nullptr;
        }
        ~_DeferredAction() {
            if (func_) { func_(); }
        }
    };

    template<typename T>
    _DeferredAction _DeferredActionCtor(T&& p) {
        return _DeferredAction(p);
    }
}

/**
* @brief calculate aligned number
* @param [in]: num: the original number that to aligned
* @param [in]: align: the align factor
* @return the number after aligned
*/
#define ALIGN_UP(num, align) (((num) + (align) - 1) & ~((align) - 1))

#define _SAFECALL(func, expected, msg, ret_val)                                                     \
  do {                                                                                              \
    int _ret = (func);                                                                              \
    if ((expected) != _ret) {                                                                       \
      LOG(ERROR) << "[InferServer] Call [" << #func << "] failed, ret = " << _ret << ". " << msg;        \
      return (ret_val);                                                                             \
                }                                                                                               \
        } while (0)

#define CUDA_SAFECALL(func, msg, ret_val) _SAFECALL(func, cudaSuccess, msg, ret_val)

#define _CALLFUNC(func, expected, msg)                                                     \
  do {                                                                                              \
    int _ret = (func);                                                                              \
    if ((expected) != _ret) {                                                                       \
      LOG(ERROR) << "[InferServer] Call [" << #func << "] failed, ret = " << _ret << ". " << msg;        \
                        }                                                                                               \
            } while (0)

#define CALL_CUDA_FUNC(func, msg) _CALLFUNC(func, cudaSuccess, msg)

inline BufSurfaceColorFormat CastColorFmt(AVPixelFormat format) {
    static std::map<AVPixelFormat, BufSurfaceColorFormat> color_map{
        { AV_PIX_FMT_NV12, BUF_COLOR_FORMAT_NV12 },
        { AV_PIX_FMT_NV21, BUF_COLOR_FORMAT_NV21 },
        { AV_PIX_FMT_RGB24, BUF_COLOR_FORMAT_RGB },
        { AV_PIX_FMT_BGR24, BUF_COLOR_FORMAT_BGR },
    };
    return color_map[format];
}

inline BufSurfaceColorFormat CastColorFmt(nvcv::ImageFormat format) {
    static std::map<nvcv::ImageFormat, BufSurfaceColorFormat> color_map{
        { nvcv::FMT_NV12, BUF_COLOR_FORMAT_NV12 },
        { nvcv::FMT_NV21, BUF_COLOR_FORMAT_NV21 },
        { nvcv::FMT_RGB8, BUF_COLOR_FORMAT_RGB },
        { nvcv::FMT_BGR8, BUF_COLOR_FORMAT_BGR },
        { nvcv::FMT_RGBf32p, BUF_COLOR_FORMAT_RGBf32p },
        { nvcv::FMT_BGRf32p, BUF_COLOR_FORMAT_BGRf32p },
    };
    return color_map[format];
}

inline nvcv::ImageFormat CastColorFmt(BufSurfaceColorFormat format) {
    static std::map<BufSurfaceColorFormat, nvcv::ImageFormat> color_map{
        { BUF_COLOR_FORMAT_NV12, nvcv::FMT_NV12 },
        { BUF_COLOR_FORMAT_NV21, nvcv::FMT_NV21 },
        { BUF_COLOR_FORMAT_RGB, nvcv::FMT_RGB8 },
        { BUF_COLOR_FORMAT_BGR, nvcv::FMT_BGR8 },
        { BUF_COLOR_FORMAT_RGBf32p, nvcv::FMT_RGBf32p },
        { BUF_COLOR_FORMAT_BGRf32p, nvcv::FMT_BGRf32p },
        { BUF_COLOR_FORMAT_TENSOR, nvcv::FMT_RGBf32p },
    };
    return color_map[format];
}

#endif  // COMMON_UTILS_HPP_
