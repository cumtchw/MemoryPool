#ifndef CUDAPROFILERTYPEDEFS_H
#define CUDAPROFILERTYPEDEFS_H

#include <cudaProfiler.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
 * Macros for the latest version for each driver function in cudaProfiler.h
 */
#define PFN_cuProfilerInitialize  PFN_cuProfilerInitialize_v4000
#define PFN_cuProfilerStart  PFN_cuProfilerStart_v4000
#define PFN_cuProfilerStop  PFN_cuProfilerStop_v4000


/**
 * Type definitions for functions defined in cudaProfiler.h
 */
typedef CUresult (CUDAAPI *PFN_cuProfilerInitialize_v4000)(const char *configFile, const char *outputFile, CUoutput_mode outputMode);
typedef CUresult (CUDAAPI *PFN_cuProfilerStart_v4000)(void);
typedef CUresult (CUDAAPI *PFN_cuProfilerStop_v4000)(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // file guard
