#ifndef CUDAEGLTYPEDEFS_H
#define CUDAEGLTYPEDEFS_H

#include <cudaEGL.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
 * Macros for the latest version for each driver function in cudaEGL.h
 */
#define PFN_cuGraphicsEGLRegisterImage  PFN_cuGraphicsEGLRegisterImage_v7000
#define PFN_cuEGLStreamConsumerConnect  PFN_cuEGLStreamConsumerConnect_v7000
#define PFN_cuEGLStreamConsumerConnectWithFlags  PFN_cuEGLStreamConsumerConnectWithFlags_v8000
#define PFN_cuEGLStreamConsumerDisconnect  PFN_cuEGLStreamConsumerDisconnect_v7000
#define PFN_cuEGLStreamConsumerAcquireFrame  PFN_cuEGLStreamConsumerAcquireFrame_v7000
#define PFN_cuEGLStreamConsumerReleaseFrame  PFN_cuEGLStreamConsumerReleaseFrame_v7000
#define PFN_cuEGLStreamProducerConnect  PFN_cuEGLStreamProducerConnect_v7000
#define PFN_cuEGLStreamProducerDisconnect  PFN_cuEGLStreamProducerDisconnect_v7000
#define PFN_cuEGLStreamProducerPresentFrame  PFN_cuEGLStreamProducerPresentFrame_v7000
#define PFN_cuEGLStreamProducerReturnFrame  PFN_cuEGLStreamProducerReturnFrame_v7000
#define PFN_cuGraphicsResourceGetMappedEglFrame  PFN_cuGraphicsResourceGetMappedEglFrame_v7000
#define PFN_cuEventCreateFromEGLSync  PFN_cuEventCreateFromEGLSync_v9000


/**
 * Type definitions for functions defined in cudaEGL.h
 */
typedef CUresult (CUDAAPI *PFN_cuGraphicsEGLRegisterImage_v7000)(CUgraphicsResource CUDAAPI *pCudaResource, EGLImageKHR image, unsigned int flags);
typedef CUresult (CUDAAPI *PFN_cuEGLStreamConsumerConnect_v7000)(CUeglStreamConnection CUDAAPI *conn, EGLStreamKHR stream);
typedef CUresult (CUDAAPI *PFN_cuEGLStreamConsumerConnectWithFlags_v8000)(CUeglStreamConnection CUDAAPI *conn, EGLStreamKHR stream, unsigned int flags);
typedef CUresult (CUDAAPI *PFN_cuEGLStreamConsumerDisconnect_v7000)(CUeglStreamConnection CUDAAPI *conn);
typedef CUresult (CUDAAPI *PFN_cuEGLStreamConsumerAcquireFrame_v7000)(CUeglStreamConnection CUDAAPI *conn, CUgraphicsResource CUDAAPI *pCudaResource, CUstream CUDAAPI *pStream, unsigned int timeout);
typedef CUresult (CUDAAPI *PFN_cuEGLStreamConsumerReleaseFrame_v7000)(CUeglStreamConnection CUDAAPI *conn, CUgraphicsResource pCudaResource, CUstream CUDAAPI *pStream);
typedef CUresult (CUDAAPI *PFN_cuEGLStreamProducerConnect_v7000)(CUeglStreamConnection CUDAAPI *conn, EGLStreamKHR stream, EGLint width, EGLint height);
typedef CUresult (CUDAAPI *PFN_cuEGLStreamProducerDisconnect_v7000)(CUeglStreamConnection CUDAAPI *conn);
typedef CUresult (CUDAAPI *PFN_cuEGLStreamProducerPresentFrame_v7000)(CUeglStreamConnection CUDAAPI *conn, CUeglFrame_v1 eglframe, CUstream CUDAAPI *pStream);
typedef CUresult (CUDAAPI *PFN_cuEGLStreamProducerReturnFrame_v7000)(CUeglStreamConnection CUDAAPI *conn, CUeglFrame_v1 CUDAAPI *eglframe, CUstream CUDAAPI *pStream);
typedef CUresult (CUDAAPI *PFN_cuGraphicsResourceGetMappedEglFrame_v7000)(CUeglFrame_v1 CUDAAPI *eglFrame, CUgraphicsResource resource, unsigned int index, unsigned int mipLevel);
typedef CUresult (CUDAAPI *PFN_cuEventCreateFromEGLSync_v9000)(CUevent CUDAAPI *phEvent, EGLSyncKHR eglSync, unsigned int flags);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // file guard
