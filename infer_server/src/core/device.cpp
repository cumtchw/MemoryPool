/*************************************************************************
* Copyright (C) [2020] by Cambricon, Inc. All rights reserved
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

#include <glog/logging.h>

#include "nvis/infer_server.h"

namespace infer_server {
    /**
    * Allocate ZeroCopy mapped memory, shared between CUDA and CPU.
    *
    * @note although two pointers are returned, one for CPU and GPU, they both resolve to the same physical memory.
    *
    * @param[out] cpuPtr Returned CPU pointer to the shared memory.
    * @param[out] gpuPtr Returned GPU pointer to the shared memory.
    * @param[in] size Size (in bytes) of the shared memory to allocate.
    *
    * @returns `0` if the allocation succeeded, otherwise faield.
    * @ingroup cudaMemory
    */
    int cudaAllocMapped(void** cpuPtr, void** gpuPtr, size_t size) {
        if (!cpuPtr || !gpuPtr || size == 0)
            return -1;

        CUDA_SAFECALL(cudaHostAlloc(cpuPtr, size, cudaHostAllocMapped), "cudaHostAlloc failed", -1);
        CUDA_SAFECALL(cudaHostGetDevicePointer(gpuPtr, *cpuPtr, 0), "cudaHostGetDevicePointer failed", -1);

        memset(*cpuPtr, 0, size);
        VLOG(3) << "[InferServer] cudaAllocMapped " << size << " bytes, CPU " << *cpuPtr << " GPU " << *gpuPtr;
        return 0;
    }


    /**
    * Allocate ZeroCopy mapped memory, shared between CUDA and CPU.
    *
    * @note this overload of cudaAllocMapped returns one pointer, assumes that the
    *       CPU and GPU addresses will match (as is the case with any recent CUDA version).
    *
    * @param[out] ptr Returned pointer to the shared CPU/GPU memory.
    * @param[in] size Size (in bytes) of the shared memory to allocate.
    *
    * @returns `0` if the allocation succeeded, otherwise failed.
    * @ingroup cudaMemory
    */
    int cudaAllocMapped(void** ptr, size_t size) {
        void* cpuPtr{};
        void* gpuPtr{};

        if (!ptr || size == 0)
            return cudaErrorInvalidValue;

        auto error = cudaAllocMapped(&cpuPtr, &gpuPtr, size);
        if (error != cudaSuccess)
            return error;

        CUDA_SAFECALL(cpuPtr != gpuPtr, "cudaAllocMapped() - addresses of CPU and GPU pointers don't match", cudaErrorMemoryAllocation);

        *ptr = gpuPtr;
        return cudaSuccess;
    }

    cudaMemcpyKind GetMemcpyKind(BufSurfaceMemType src_mem_type, BufSurfaceMemType dst_mem_type) {
        // 确保未注册内存不会被使用
        assert(src_mem_type != BUF_MEMORY_UNREGISTERED);
        assert(dst_mem_type != BUF_MEMORY_UNREGISTERED);

        // 根据源和目标内存类型确定 cudaMemcpyKind
        if (src_mem_type == BUF_MEMORY_HOST && dst_mem_type == BUF_MEMORY_HOST) {
            return cudaMemcpyHostToHost;
        }
        else if (src_mem_type == BUF_MEMORY_HOST && dst_mem_type == BUF_MEMORY_DEVICE) {
            return cudaMemcpyHostToDevice;
        }
        else if (src_mem_type == BUF_MEMORY_DEVICE && dst_mem_type == BUF_MEMORY_HOST) {
            return cudaMemcpyDeviceToHost;
        }
        else if (src_mem_type == BUF_MEMORY_DEVICE && dst_mem_type == BUF_MEMORY_DEVICE) {
            return cudaMemcpyDeviceToDevice;
        }
        else if (src_mem_type == BUF_MEMORY_MAP || dst_mem_type == BUF_MEMORY_MAP) {
            // 管理内存可以视为主机或设备内存
            return cudaMemcpyDefault;
        }

        // 默认情况下返回 cudaMemcpyDefault
        return cudaMemcpyDefault;
    }

    bool SetCurrentDevice(int device_id) noexcept{
        CUDA_SAFECALL(cudaSetDevice(device_id), "Set device failed", false);
        VLOG(3) << "[InferServer] SetCurrentDevice(): Set device [" << device_id << "] for this thread";
        return true;
    }

    int GetCurrentDevice(int& device_id) noexcept{
        CUDA_SAFECALL(cudaGetDevice(&device_id), "Get current divice failed", -1);
        VLOG(3) << "[InferServer] GetCurrentDevice(): Get device [" << device_id << "] in this thread";
        return 0;
    }

    uint32_t TotalDeviceCount() noexcept{
        int dev_cnt;
        CUDA_SAFECALL(cudaGetDeviceCount(&dev_cnt), "Set device failed", 0);
        return static_cast<uint32_t>(dev_cnt);;
    }

    bool CheckDevice(int device_id) noexcept{
        int dev_cnt;
        CUDA_SAFECALL(cudaGetDeviceCount(&dev_cnt), "Check device failed", false);
        return device_id < static_cast<int>(dev_cnt) && device_id >= 0;
    }

    void* MallocDeviceMem(size_t size) noexcept{
        void *device_ptr{};
        if (cudaAllocMapped(&device_ptr, size) != 0)
            return nullptr;
       
        return device_ptr;
    }

    int FreeDeviceMem(void *p) noexcept{
        CUDA_SAFECALL(cudaFreeHost(p), "Free device memory failed", -1);
        return 0;
    }

    void* MallocHostMem(size_t size) noexcept{
        void *host_ptr{};
        CUDA_SAFECALL(cudaMallocHost(&host_ptr, size), "Malloc host memory failed", nullptr);
        return host_ptr;
    }

    int FreeHostMem(void *p) noexcept{
        CUDA_SAFECALL(cudaFreeHost(p), "Free host memory failed", -1);
        return 0;
    }

    int MemcpyHD(void* dst, BufSurfaceMemType dst_mem_type, void* src, BufSurfaceMemType src_mem_type, size_t size) noexcept{
        cudaMemcpyKind cpy_type = GetMemcpyKind(src_mem_type, dst_mem_type);
        int ret = cudaMemcpy(dst, src, size, cpy_type);
        CUDA_SAFECALL(cudaMemcpy(dst, src, size, cpy_type), "Memcpy HD failed", -1);
        return 0;
    }

    bool IsItegratedGPU(int device_id) {
        cudaDeviceProp prop;
        CUDA_SAFECALL(cudaGetDeviceProperties(&prop, device_id), "not ItegratedGPU", 0);
        return prop.integrated;
    }

}  // namespace infer_server
