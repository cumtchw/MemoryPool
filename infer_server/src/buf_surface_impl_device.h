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

#ifndef BUF_SURFACE_IMPL_DEVICE_H_
#define BUF_SURFACE_IMPL_DEVICE_H_

#include <string>
#include <mutex>

#include "buf_surface_impl.h"

namespace infer_server {

    class MemAllocatorDevice : public IMemAllcator {
    public:
        MemAllocatorDevice() = default;
        ~MemAllocatorDevice() = default;
        int Create(BufSurfaceCreateParams *params) override;
        int Destroy() override;
        int Alloc(BufSurface *surf) override;
        int Free(BufSurface *surf) override;

    private:
        bool created_{};
        BufSurfaceCreateParams create_params_;
        uint32_t width_stride_{}, height_stride_{};
        uint32_t block_size_{};
    };

}  // namespace 

#endif  // BUF_SURFACE_IMPL_DEVICE_H_
