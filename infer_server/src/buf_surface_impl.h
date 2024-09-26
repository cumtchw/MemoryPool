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

#ifndef BUF_SURFACE_IMPL_H_
#define BUF_SURFACE_IMPL_H_

#include <string>
#include <mutex>
#include <queue>

#include "buf_surface.h"
#include "buf_surface_utils.h"

namespace infer_server {

class IMemAllcator {
 public:
  virtual ~IMemAllcator() {}
  virtual int Create(BufSurfaceCreateParams *params) = 0;
  virtual int Destroy() = 0;
  virtual int Alloc(BufSurface *surf) = 0;
  virtual int Free(BufSurface *surf) = 0;
};

IMemAllcator *CreateMemAllocator(BufSurfaceMemType mem_type);

class MemPool {
 public:
  MemPool() = default;
  ~MemPool() {
    if (allocator_) delete allocator_, allocator_ = nullptr;
  }
  int Create(BufSurfaceCreateParams *params, uint32_t block_num);
  int Destroy();
  int Alloc(BufSurface *surf);
  int Free(BufSurface *surf);

 private:
  std::mutex mutex_;
  std::queue<BufSurface> cache_;

  bool created_ = false;
  int device_id_ = 0;
  uint32_t alloc_count_ = 0;
  IMemAllcator *allocator_ = nullptr;
};

//  for non-pool case
int CreateSurface(BufSurfaceCreateParams *params, BufSurface *surf);
int DestroySurface(BufSurface *surf);

}  // namespace 

#endif  // BUF_SURFACE_IMPL_H_
