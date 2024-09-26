#ifndef CUDAVDPAUTYPEDEFS_H
#define CUDAVDPAUTYPEDEFS_H

// Dependent includes for cudavdpau.h
#include <vdpau/vdpau.h>

#include <cudaVDPAU.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
 * Macros for the latest version for each driver function in cudaVDPAU.h
 */
#define PFN_cuVDPAUGetDevice  PFN_cuVDPAUGetDevice_v3010
#define PFN_cuVDPAUCtxCreate  PFN_cuVDPAUCtxCreate_v3020
#define PFN_cuGraphicsVDPAURegisterVideoSurface  PFN_cuGraphicsVDPAURegisterVideoSurface_v3010
#define PFN_cuGraphicsVDPAURegisterOutputSurface  PFN_cuGraphicsVDPAURegisterOutputSurface_v3010


/**
 * Type definitions for functions defined in cudaVDPAU.h
 */
typedef CUresult (CUDAAPI *PFN_cuVDPAUGetDevice_v3010)(CUdevice_v1 *pDevice, VdpDevice vdpDevice, VdpGetProcAddress *vdpGetProcAddress);
typedef CUresult (CUDAAPI *PFN_cuVDPAUCtxCreate_v3020)(CUcontext *pCtx, unsigned int flags, CUdevice_v1 device, VdpDevice vdpDevice, VdpGetProcAddress *vdpGetProcAddress);
typedef CUresult (CUDAAPI *PFN_cuGraphicsVDPAURegisterVideoSurface_v3010)(CUgraphicsResource *pCudaResource, VdpVideoSurface vdpSurface, unsigned int flags);
typedef CUresult (CUDAAPI *PFN_cuGraphicsVDPAURegisterOutputSurface_v3010)(CUgraphicsResource *pCudaResource, VdpOutputSurface vdpSurface, unsigned int flags);

/*
 * Type definitions for older versioned functions in cudaVDPAU.h
 */
#if defined(__CUDA_API_VERSION_INTERNAL)
typedef CUresult (CUDAAPI *PFN_cuVDPAUCtxCreate_v3010)(CUcontext *pCtx, unsigned int flags, CUdevice_v1 device, VdpDevice vdpDevice, VdpGetProcAddress *vdpGetProcAddress);
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // file guard
