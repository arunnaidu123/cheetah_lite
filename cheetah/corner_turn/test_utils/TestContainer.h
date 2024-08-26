/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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

#ifndef SKA_CHEETAH_CORNER_TURN_TEST_UTILS_TESTCONTAINER_H
#define SKA_CHEETAH_CORNER_TURN_TEST_UTILS_TESTCONTAINER_H

#include "panda/DeviceMemory.h"

namespace ska {
namespace cheetah {
namespace corner_turn {
namespace test {

/**
 * @brief A contiguous memory block allocation for use with the CornerTester
 * @tparam Arch The architecture of the device on which to allocate memory
 */
template <class Arch, typename NumericalT>
class TestContainer
{
    public:
        typedef panda::DeviceMemory<Arch, NumericalT> ContainerType;
        typedef typename ContainerType::Iterator Iterator;
        typedef typename ContainerType::ConstIterator ConstIterator;

    public:

        TestContainer(size_t size);
        ~TestContainer();
        /**
         * @brief Return an iterator pointing to the begin of the container
         */
        Iterator begin();
        ConstIterator begin() const;
        ConstIterator cbegin() const;

        /**
         * @brief Return an iterator pointing to the end of the container
         */
        Iterator end();
        ConstIterator end() const;
        ConstIterator cend() const;

    private:
        ContainerType _data;
};

} // namespace test
} // namespace corner_turn
} // namespace cheetah
} // namespace ska
#include "cheetah/corner_turn/test_utils/detail/TestContainer.cpp"

#endif // SKA_CHEETAH_CORNER_TURN_TEST_UTILS_TESTCONTAINER_H
