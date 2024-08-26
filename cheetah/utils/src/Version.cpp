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
#include "panda/Version.h"
#include "cheetah/Configuration.h"
#include "cheetah/version.h"
#ifdef SKA_CHEETAH_ENABLE_SKA_RABBIT
#include "rabbit/Version.h"
#endif // SKA_CHEETAH_ENABLE_SKA_RABBIT

#define _to_string(_str_) #_str_
#define to_string(_str_) _to_string(_str_)
#define compare_version(_dep_) static_assert(CHEETAH_##_dep_ == PANDA_##_dep_, to_string(_dep_ mismatch) " between cheetah and panda ( " to_string(CHEETAH_##_dep_) " vs " to_string( PANDA_##_dep_) " )")

namespace ska {
namespace cheetah {

const char* version=to_string(CHEETAH_VERSION);
const char* full_version=CHEETAH_VERSION " (" CHEETAH_RCS_VERSION ")";
const char* boost_version=to_string(CHEETAH_BOOST_VERSION);
const char* cuda_version=to_string(CHEETAH_CUDA_VERSION);
const char* panda_version=to_string(PANDA_VERSION);

compare_version(BOOST_MAJOR_VERSION);
compare_version(BOOST_MINOR_VERSION);
static_assert(CHEETAH_BOOST_ASIO_DEBUG == PANDA_BOOST_ASIO_DEBUG, "BOOST_ASIO_DEBUG mistmatch between panda and cheetah");
#ifdef SKA_CHEETAH_ENABLE_CUDA
compare_version(CUDA_MAJOR_VERSION);
compare_version(CUDA_MINOR_VERSION);
#endif // SKA_CHEETAH_ENABLE_CUDA

} // namespace cheetah
} // namespace ska
