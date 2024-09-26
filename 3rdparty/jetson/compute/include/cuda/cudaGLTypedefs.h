#ifndef CUDAGLTYPEDEFS_H
#define CUDAGLTYPEDEFS_H

// Dependent includes for cudagl.h
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <cudaGL.h>

#if defined(CUDA_API_PER_THREAD_DEFAULT_STREAM)
    #define __API_TYPEDEF_PTDS(api, default_version, ptds_version) api ## _v ## ptds_version ## _ptds
    #define __API_TYPEDEF_PTSZ(api, default_version, ptds_version) api ## _v ## ptds_version ## _ptsz
#else
    #define __API_TYPEDEF_PTDS(api, default_version, ptds_version) api ## _v ## default_version
    #define __API_TYPEDEF_PTSZ(api, default_version, ptds_version) api ## _v ## default_version
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
 * Macros for the latest version for each driver function in cudaGL.h
 */
#define PFN_cuGraphicsGLRegisterBuffer  PFN_cuGraphicsGLRegisterBuffer_v3000
#define PFN_cuGraphicsGLRegisterImage  PFN_cuGraphicsGLRegisterImage_v3000
#define PFN_cuWGLGetDevice  PFN_cuWGLGetDevice_v2020
#define PFN_cuGLGetDevices  PFN_cuGLGetDevices_v6050
#define PFN_cuGLCtxCreate  PFN_cuGLCtxCreate_v3020
#define PFN_cuGLInit  PFN_cuGLInit_v2000
#define PFN_cuGLRegisterBufferObject  PFN_cuGLRegisterBufferObject_v2000
#define PFN_cuGLMapBufferObject  __API_TYPEDEF_PTDS(PFN_cuGLMapBufferObject, 3020, 7000)
#define PFN_cuGLUnmapBufferObject  PFN_cuGLUnmapBufferObject_v2000
#define PFN_cuGLUnregisterBufferObject  PFN_cuGLUnregisterBufferObject_v2000
#define PFN_cuGLSetBufferObjectMapFlags  PFN_cuGLSetBufferObjectMapFlags_v2030
#define PFN_cuGLMapBufferObjectAsync  __API_TYPEDEF_PTSZ(PFN_cuGLMapBufferObjectAsync, 3020, 7000)
#define PFN_cuGLUnmapBufferObjectAsync  PFN_cuGLUnmapBufferObjectAsync_v2030


/**
 * Type definitions for functions defined in cudaGL.h
 */
typedef CUresult (CUDAAPI *PFN_cuGraphicsGLRegisterBuffer_v3000)(CUgraphicsResource *pCudaResource, GLuint buffer, unsigned int Flags);
typedef CUresult (CUDAAPI *PFN_cuGraphicsGLRegisterImage_v3000)(CUgraphicsResource *pCudaResource, GLuint image, GLenum target, unsigned int Flags);
#ifdef _WIN32
typedef CUresult (CUDAAPI *PFN_cuWGLGetDevice_v2020)(CUdevice_v1 *pDevice, HGPUNV hGpu);
#endif
typedef CUresult (CUDAAPI *PFN_cuGLGetDevices_v6050)(unsigned int *pCudaDeviceCount, CUdevice_v1 *pCudaDevices, unsigned int cudaDeviceCount, CUGLDeviceList deviceList);
typedef CUresult (CUDAAPI *PFN_cuGLCtxCreate_v3020)(CUcontext *pCtx, unsigned int Flags, CUdevice_v1 device);
typedef CUresult (CUDAAPI *PFN_cuGLInit_v2000)(void);
typedef CUresult (CUDAAPI *PFN_cuGLRegisterBufferObject_v2000)(GLuint buffer);
typedef CUresult (CUDAAPI *PFN_cuGLMapBufferObject_v7000_ptds)(CUdeviceptr_v2 *dptr, size_t *size, GLuint buffer);
typedef CUresult (CUDAAPI *PFN_cuGLUnmapBufferObject_v2000)(GLuint buffer);
typedef CUresult (CUDAAPI *PFN_cuGLUnregisterBufferObject_v2000)(GLuint buffer);
typedef CUresult (CUDAAPI *PFN_cuGLSetBufferObjectMapFlags_v2030)(GLuint buffer, unsigned int Flags);
typedef CUresult (CUDAAPI *PFN_cuGLMapBufferObjectAsync_v7000_ptsz)(CUdeviceptr_v2 *dptr, size_t *size, GLuint buffer, CUstream hStream);
typedef CUresult (CUDAAPI *PFN_cuGLUnmapBufferObjectAsync_v2030)(GLuint buffer, CUstream hStream);
typedef CUresult (CUDAAPI *PFN_cuGLMapBufferObject_v3020)(CUdeviceptr_v2 *dptr, size_t *size, GLuint buffer);
typedef CUresult (CUDAAPI *PFN_cuGLMapBufferObjectAsync_v3020)(CUdeviceptr_v2 *dptr, size_t *size, GLuint buffer, CUstream hStream);

/*
 * Type definitions for older versioned functions in cuda.h
 */
#if defined(__CUDA_API_VERSION_INTERNAL)
typedef CUresult (CUDAAPI *PFN_cuGLGetDevices_v4010)(unsigned int *pCudaDeviceCount, CUdevice_v1 *pCudaDevices, unsigned int cudaDeviceCount, CUGLDeviceList deviceList);
typedef CUresult (CUDAAPI *PFN_cuGLMapBufferObject_v2000)(CUdeviceptr_v1 *dptr, unsigned int *size, GLuint buffer);
typedef CUresult (CUDAAPI *PFN_cuGLMapBufferObjectAsync_v2030)(CUdeviceptr_v1 *dptr, unsigned int *size, GLuint buffer, CUstream hStream);
typedef CUresult (CUDAAPI *PFN_cuGLCtxCreate_v2000)(CUcontext *pCtx, unsigned int Flags, CUdevice_v1 device);
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // file guard
