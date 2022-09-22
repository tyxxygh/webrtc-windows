/* Copyright (c) 2016-2017, NVIDIA CORPORATION. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef NVPIPE_H_
#define NVPIPE_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

/** Codecs usable for the encoding/decoding session */
typedef enum nvpipe_codec {
    NVPIPE_H264_NV, /**< NVIDIA video codec SDK backend */
    NVPIPE_H264_NVFFMPEG, /**< NVIDIA-based ffmpeg backend */
    NVPIPE_H264_FFMPEG, /**< CPU-based ffmpeg backend */
} nvp_codec_t;

/** Supported NvPipe image formats. */
typedef enum nvpipe_format {
    NVPIPE_RGB,
    NVPIPE_RGBA,
} nvp_fmt_t;

/* Avoid a dependency on cuda.h by copying these definitions here. */
#define cuda_SUCCESS 0
#define cuda_ERROR_INVALID_VALUE 11
#define cuda_ERROR_OUT_OF_MEMORY 2
#define cuda_ERROR_MAP_FAILED 14
#define cuda_ERROR_UNMAP_FAILED 15
#define cuda_ERROR_FILE_NOT_FOUND 33
#define cuda_ERROR_UNKNOWN 10000

/** NvPipe error codes are a superset of the CUDA error codes.  See
 * nvpipe_strerror. */
typedef enum nvpipe_error_code {
    NVPIPE_SUCCESS = cuda_SUCCESS,
    NVPIPE_EINVAL = cuda_ERROR_INVALID_VALUE,
    NVPIPE_ENOMEM = cuda_ERROR_OUT_OF_MEMORY,
    NVPIPE_EMAP = cuda_ERROR_MAP_FAILED,
    NVPIPE_EUNMAP = cuda_ERROR_UNMAP_FAILED,
    NVPIPE_ENOENT = cuda_ERROR_FILE_NOT_FOUND,
    NVPIPE_EENCODE = cuda_ERROR_UNKNOWN+1,
    NVPIPE_EDECODE = cuda_ERROR_UNKNOWN+2,
    NVPIPE_EOVERFLOW = cuda_ERROR_UNKNOWN+3,
    NVPIPE_EAGAIN = cuda_ERROR_UNKNOWN+4,
} nvp_err_t;

#ifndef _WIN32
    #ifdef __LP64__
         #define __stdcall
    #else
        #define __stdcall __attribute__((__stdcall__))
    #endif //__LP64__
#endif//_WIN32

typedef void nvpipe;

typedef nvpipe*(__stdcall *nvpipe_create_encoder)(nvp_codec_t id, uint64_t bitrate, uint64_t frameRate, uint64_t idrPeriod, uint64_t intraRefreshPeriod, bool intraRefreshEnableFlag);
typedef	void(__stdcall *nvpipe_destroy)(nvpipe* const __restrict codec);
typedef	nvp_err_t(__stdcall *nvpipe_encode)(nvpipe * const __restrict codec,
	const void *const __restrict ibuf,
	const size_t ibuf_sz,
	void *const __restrict obuf,
	size_t* const __restrict obuf_sz,
	const uint32_t width, const uint32_t height, const uint32_t frameRate,
	nvp_fmt_t format);
typedef	nvp_err_t(__stdcall *nvpipe_bitrate)(nvpipe* const enc, uint64_t bitrate, uint64_t frameRate);

#  ifdef __cplusplus
}
#  endif
#endif
