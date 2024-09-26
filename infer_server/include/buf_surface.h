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

/**
 * @file buf_surface.h
 * <b>BufSurface Interface </b>
 *
 * This file specifies the BufSurface management API.
 *
 * The BufSurface API provides methods to allocate / deallocate and copy batched buffers.
 */

#ifndef BUF_SURFACE_H_
#define BUF_SURFACE_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Defines the default padding length for reserved fields of structures. */
#define PADDING_LENGTH  4

/**
 * Specifies color formats for \ref BufSurface.
 */
typedef enum {
  /** Specifies an invalid color format. */
  BUF_COLOR_FORMAT_INVALID,
  /** Specifies BT.601 colorspace - Y/CbCr 4:2:0 multi-planar. */
  BUF_COLOR_FORMAT_NV12,
  /** Specifies BT.601 colorspace - Y/CbCr 4:2:0 multi-planar. */
  BUF_COLOR_FORMAT_NV21,
  /** Specifies RGB-8-8-8 single plane. */
  BUF_COLOR_FORMAT_RGB,
  /** Specifies BGR-8-8-8 single plane. */
  BUF_COLOR_FORMAT_BGR,
  /** Planar RGB unsigned float32 per channel. */
  BUF_COLOR_FORMAT_RGBf32p,
  /** Planar BGR unsigned float32 per channel. */
  BUF_COLOR_FORMAT_BGRf32p,

  /** for inference*/
  BUF_COLOR_FORMAT_TENSOR,
  BUF_COLOR_FORMAT_LAST,
} BufSurfaceColorFormat;

/**
 * Specifies memory types for \ref BufSurface.
 */
typedef enum {
    /** Unregistered memory*/
  BUF_MEMORY_UNREGISTERED,
  /** Host memory.*/
  BUF_MEMORY_HOST,
  /** Device memory*/
  BUF_MEMORY_DEVICE,
  /** Managed memory*/
  BUF_MEMORY_MAP,
} BufSurfaceMemType;

/**
 * Holds parameters required to allocate an \ref BufSurface.
 */
typedef struct BufSurfaceCreateParams {
  /** Holds the type of memory to be allocated. */
  BufSurfaceMemType mem_type;
  /** Holds the Device ID. */
  int device_id;
  /** Holds the width of the buffer. */
  uint32_t width;
  /** Holds the height of the buffer. */
  uint32_t height;
  /** Holds the width stride bytes of the buffer. */
  uint32_t width_stride;
  /** Holds the height stride bytes of the buffer. */
  uint32_t height_stride;
  /** Holds the color format of the buffer. */
  BufSurfaceColorFormat color_format;

  /** Holds the amount of memory to be allocated. Optional; if set, all other
   parameters (width, height, etc.) are ignored. */
  uint32_t size;

  /** Holds the batch size. */
  uint32_t batch_size;

  /** Holds the alignment mode, if set,  1 bytes alignment will be applied; */
  bool force_align_1;

  void *_reserved[PADDING_LENGTH];
} BufSurfaceCreateParams;

/**
 * Holds information about a single buffer in a batch.
 */
typedef struct BufSurfaceParams {
  /** Holds the width of the buffer. */
  int32_t width;
  /** Holds the height of the buffer. */
  int32_t height;
  /** Holds the width stride bytes of the buffer. */
  int32_t width_stride;
  /** Holds the height stride bytes of the buffer. */
  int32_t height_stride;
  /** Holds the color format of the buffer. */
  BufSurfaceColorFormat color_format;

  /** Holds the amount of allocated memory. */
  int32_t data_size;

  /** Holds a pointer to allocated memory. */
  void * data_ptr;

  /** Holds a pointer to a CPU mapped buffer. */
  void * mapped_data_ptr;

  void * _reserved[PADDING_LENGTH];
} BufSurfaceParams;

/**
 * Holds information about batched buffers.
 */
typedef struct BufSurface {
  /** Holds type of memory for buffers in the batch. */
  BufSurfaceMemType mem_type;

  /** Holds a Device ID. */
  int device_id;
  /** Holds the batch size. */
  uint32_t batch_size;
  /** Holds the number valid and filled buffers. Initialized to zero when
   an instance of the structure is created. */
  uint32_t num_filled;

  /** Holds an "is contiguous" flag. If set, memory allocated for the batch is contiguous.  */
  bool is_contiguous;

  /** Holds a pointer to an array of batched buffers. */
  BufSurfaceParams *surface_list;

  /** Holds a pointer to the buffer pool context */
  void *opaque;

  /** Holds the timestamp for video image, valid only for batch_size == 1 */
  uint64_t pts;

  void * _reserved[PADDING_LENGTH];
} BufSurface;

/**
 * @brief  Creates a Buffer Pool.
 *
 * Call BufPoolDestroy() to free resources allocated by this function.
 *
 * @param[out] pool         An indirect pointer to the buffer pool.
 * @param[in]  params       A pointer to an \ref BufSurfaceCreateParams
 *                           structure.
 * @param[in]  block_num    The block number.
 *
 * @return Returns 0 if this function has run successfully. Otherwise returns -1.
 */
int BufPoolCreate(void **pool, BufSurfaceCreateParams *params, uint32_t block_num);

/**
 * @brief  Frees the buffer pool previously allocated by BufPoolCreate().
 *
 * @param[in] surf  A pointer to an \ref buffer pool to be freed.
 *
 * @return Returns 0 if this function has run successfully. Otherwise returns -1.
 */
int BufPoolDestroy(void *pool);

/**
 * @brief  Allocates a single buffer.
 *
 * Allocates memory for a buffer and returns a pointer to an
 * allocated \ref BufSurface. The \a params structure must have
 * the allocation parameters of a single buffer.
 *
 * Call BufSurfaceDestroy() to free resources allocated by this function.
 *
 * @param[out] surf         An indirect pointer to the allocated buffer.
 * @param[in]  pool         A pointer to a buffer pool.
 *
 * @return Returns 0 if this function has run successfully. Otherwise returns -1.
 */
int BufSurfaceCreateFromPool(BufSurface **surf, void *pool);

/**
 * @brief  Allocates a batch of buffers.
 *
 * Allocates memory for \a batch_size buffers and returns a pointer to an
 * allocated \ref BufSurface. The \a params structure must have
 * the allocation parameters. If \a params.size
 * is set, a buffer of that size is allocated, and all other
 * parameters (width, height, color format, etc.) are ignored.
 *
 * Call BufSurfaceDestroy() to free resources allocated by this function.
 *
 * @param[out] surf         An indirect pointer to the allocated batched
 *                           buffers.
 * @param[in]  params       A pointer to an \ref BufSurfaceCreateParams
 *                           structure.
 *
 * @return Returns 0 if this function has run successfully. Otherwise returns -1.
 */
int BufSurfaceCreate(BufSurface **surf, BufSurfaceCreateParams *params);

/**
 * @brief  Frees a single buffer allocated by BufSurfaceCreate()
 *         or batched buffers previously allocated by BufSurfaceCreate().
 *
 * @param[in] surf  A pointer to an \ref BufSurface to be freed.
 *
 * @return Returns 0 if this function has run successfully. Otherwise returns -1.
 */
int BufSurfaceDestroy(BufSurface *surf);

/**
 * @brief  Copies the content of source batched buffer(s) to destination
 * batched buffer(s).
 *
 * The source and destination \ref BufSurface objects must have same
 * buffer and batch size.
 *
 * @param[in] src_surf   A pointer to the source BufSurface structure.
 * @param[out] dst_surf   A pointer to the destination BufSurface structure.
 *
 * @return Returns 0 if this function has run successfully. Otherwise returns -1.
 */
int BufSurfaceCopy(BufSurface *src_surf, BufSurface *dst_surf);

/**
 * @brief  Fills each byte of the buffer(s) in an \ref BufSurface with a
 * provided value.
 *
 * You can also use this function to reset the buffer(s) in the batch.
 *
 * @param[in] surf  A pointer to the BufSurface structure.
 * @param[in] index Index of a buffer in the batch. -1 refers to all buffers
 *                  in the batch.
 * @param[in] value The value to be used as fill.
 *
 * @return Returns 0 if this function has run successfully. Otherwise returns -1.
 */
int BufSurfaceMemSet(BufSurface *surf, int index, uint8_t value);

/** @} */

#ifdef __cplusplus
}
#endif

#endif  // BUF_SURFACE_H_
