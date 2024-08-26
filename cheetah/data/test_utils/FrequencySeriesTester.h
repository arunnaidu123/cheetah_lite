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
#ifndef SKA_CHEETAH_DATA_TEST_UTILS_FREQUENCYSERIESTESTER_H
#define SKA_CHEETAH_DATA_TEST_UTILS_FREQUENCYSERIESTESTER_H

#include "cheetah/data/FrequencySeries.h"
#include <panda/test/TestDevice.h>
#include <gtest/gtest.h>

namespace ska {
namespace cheetah {
namespace data {
namespace test {

/**
 * @brief class to test the FrequencySeries class
 * @details
 */

template<typename FrequencySeriesT, typename OtherFrequencySeriesT>
class FrequencySeriesTesterTraits
{
    public:
        typedef FrequencySeriesT FrequencySeriesType;
        typedef OtherFrequencySeriesT OtherFrequencySeriesType;
        typedef typename FrequencySeriesType::Architecture Arch;
        typedef typename FrequencySeriesType::Allocator Allocator;

    public:
        virtual ~FrequencySeriesTesterTraits() {}

        // override this function for specialist Allocators
        virtual Allocator allocator(panda::PoolResource<Arch>& device);
};

template<typename FrequencySeriesTesterTraitsT>
class FrequencySeriesTester : public ::testing::TestWithParam<FrequencySeriesTesterTraitsT>
{
        typedef ::testing::TestWithParam<FrequencySeriesTesterTraitsT> BaseT;
        typedef panda::test::TestDevice<typename FrequencySeriesTesterTraitsT::Arch> TestSystem;

    public:
        FrequencySeriesTester();

        TestSystem& system();

    protected:
        TestSystem _system;
};

TYPED_TEST_SUITE_P(FrequencySeriesTester);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
#include "detail/FrequencySeriesTester.cpp"

#endif // SKA_CHEETAH_DATA_TEST_UTILS_FREQUENCYSERIESTESTER_H
