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
#ifndef SKA_CHEETAH_UTILS_MOCK_H
#define SKA_CHEETAH_UTILS_MOCK_H

#include "cheetah/utils/Architectures.h"

namespace ska {
namespace cheetah {
namespace utils {

/**
 * @brief Dummy class that mocks arbitrary constructors
 *
 * @details This class is intended for use in reimplementing device-specific classes
 * such as implementing dummy classes for cuda-only transforms.
 *
 * @example For the modules::tdrt::cuda::Tdrt class, the generic reimplementation comes via
 * @code
 * using Tdrt = utils::Mock<Config>
 * @endcode
 */

template <typename Arch, typename... Args>
class Mock
{
    public:
        typedef Arch Architecture;

    public:
        Mock(Args&&...);
        ~Mock();

        template<typename... OperatorArgs>
        void operator()(OperatorArgs&&...);
        
};


} // namespace utils
} // namespace cheetah
} // namespace ska
#include "cheetah/utils/detail/Mock.cpp"

#endif // SKA_CHEETAH_UTILS_MOCK_H
