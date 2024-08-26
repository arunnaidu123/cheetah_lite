/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 The SKA organisation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef SKA_CHEETAH_CUDA_UTILS_ERRORHANDLING_H
#define SKA_CHEETAH_CUDA_UTILS_ERRORHANDLING_H

#include "cheetah/Configuration.h"
#ifdef SKA_CHEETAH_ENABLE_CUDA
#include <cuda.h>
#include <cuda_runtime.h>
#include <cufft.h>
#include <sstream>
#include <stdexcept>

/**
 * @brief Cuda errorhandling and helpers
 * @details Tentative solution by include from cuda sdk samples.
 *  Set path at cmake call with -DCUDA_SMP_INC
 */
//#include "helper_cuda.h" //remove this dependency

/**
 * @brief Prefix for aligned message output in unittests when using cout
 */
#define CU_MSG "[ cuda msg ] "

/**
 * @brief Macro function for error checking on cuda calls that return cudaError_t values
 * @details This macro wrapps the cuda_assert_success function which raises a
 *  std::runtime_error upon receiving any cudaError_t value that is not cudaSuccess.
 * @example CUDA_ERROR_CHECK(cudaDeviceSynchronize());
 *  CUDA_ERROR_CHECK(cudaStreamSynchronize(stream));
 */
#define CUDA_ERROR_CHECK(ans) { cuda_assert_success((ans), __FILE__, __LINE__); }

/**
 * @brief Function that raises an error on receipt of any cudaError_t
 *  value that is not cudaSuccess
 */
//inline void cuda_assert_success(cudaError_t code, const char *file, int line)
inline void cuda_assert_success(cudaError_t code, const char *file, int line)
{
    if (code != cudaSuccess)
	{
	    /* Ewan note 28/07/2015:
	     * This stringstream needs to be made safe.
	     * Error message formatting needs to be defined.
	     */
	    std::stringstream error_msg;
	    error_msg << "CUDA failed with error: "
		      << cudaGetErrorString(code) << std::endl
		      << "File: " << file << std::endl
		      << "Line: " << line << std::endl;
	    throw std::runtime_error(error_msg.str());
	}
}

/**
 * @brief Macro function for error checking on cufft calls that return cufftResult values
 */
#define CUFFT_ERROR_CHECK(ans) { cufft_assert_success((ans), __FILE__, __LINE__); }

/**
 * @brief Function that raises an error on receipt of any cufftResult
 * value that is not CUFFT_SUCCESS
 */
inline void cufft_assert_success(cufftResult code, const char *file, int line)
{
    if (code != CUFFT_SUCCESS)
	{
	    std::stringstream error_msg;
	    error_msg << "CUFFT failed with error: ";
	    switch (code)
		{
		case CUFFT_INVALID_PLAN:
		    error_msg <<  "CUFFT_INVALID_PLAN";
		    break;

		case CUFFT_ALLOC_FAILED:
		    error_msg <<  "CUFFT_ALLOC_FAILED";
		    break;

		case CUFFT_INVALID_TYPE:
		    error_msg <<  "CUFFT_INVALID_TYPE";
		    break;

		case CUFFT_INVALID_VALUE:
		    error_msg <<  "CUFFT_INVALID_VALUE";
		    break;

		case CUFFT_INTERNAL_ERROR:
		    error_msg <<  "CUFFT_INTERNAL_ERROR";
		    break;

		case CUFFT_EXEC_FAILED:
		    error_msg <<  "CUFFT_EXEC_FAILED";
		    break;

		case CUFFT_SETUP_FAILED:
		    error_msg <<  "CUFFT_SETUP_FAILED";
		    break;

		case CUFFT_INVALID_SIZE:
		    error_msg <<  "CUFFT_INVALID_SIZE";
		    break;

		case CUFFT_UNALIGNED_DATA:
		    error_msg <<  "CUFFT_UNALIGNED_DATA";
		    break;

		default:
		    error_msg <<  "CUFFT_UNKNOWN_ERROR";
	    }
	    error_msg << std::endl
		      << "File: " << file << std::endl
		      << "Line: " << line << std::endl;
	    throw std::runtime_error(error_msg.str());
	}
}
#endif // SKA_CHEETAH_ENABLE_CUDA

#endif //SKA_CHEETAH_CUDA_UTILS_ERRORHANDLING_H
