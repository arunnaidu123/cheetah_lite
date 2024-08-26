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
#include "cheetah/data/cuda/test/FrequencyTimeTest.h"
#include "cheetah/data/test_utils/CommonCopyAndTransposeTester.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/FrequencyTime.h"
#include "panda/Copy.h"
#include <random>

namespace ska {
namespace cheetah {
namespace data {
namespace cuda {
namespace test {

template <typename T>
FrequencyTimeTest<T>::FrequencyTimeTest()
    : ::testing::Test()
{
}

template <typename T>
FrequencyTimeTest<T>::~FrequencyTimeTest()
{
}

template <typename T>
void FrequencyTimeTest<T>::SetUp()
{
}

template <typename T>
void FrequencyTimeTest<T>::TearDown()
{
}

} // namespace test
} // namespace cuda
} // namespace data
} // namespace cheetah
} // namespace ska

namespace ska {
namespace cheetah {
namespace data {
namespace test {

typedef ::testing::Types
    < CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint8_t>, data::TimeFrequency<Cuda, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint16_t>, data::TimeFrequency<Cuda, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, float>, data::TimeFrequency<Cuda, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint8_t>, data::TimeFrequency<Cuda, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint16_t>, data::TimeFrequency<Cuda, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, float>, data::TimeFrequency<Cuda, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint8_t>, data::TimeFrequency<Cuda, float>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint16_t>, data::TimeFrequency<Cuda, float>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, float>, data::TimeFrequency<Cuda, float>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint8_t>, data::FrequencyTime<Cuda, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint16_t>, data::FrequencyTime<Cuda, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, float>, data::FrequencyTime<Cuda, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint8_t>, data::FrequencyTime<Cuda, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint16_t>, data::FrequencyTime<Cuda, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, float>, data::FrequencyTime<Cuda, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint8_t>, data::FrequencyTime<Cuda, float>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, uint16_t>, data::FrequencyTime<Cuda, float>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cpu, float>, data::FrequencyTime<Cuda, float>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, uint8_t>, data::TimeFrequency<Cuda, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, uint16_t>, data::TimeFrequency<Cuda, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, float>, data::TimeFrequency<Cuda, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, uint8_t>, data::TimeFrequency<Cuda, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, uint16_t>, data::TimeFrequency<Cuda, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, float>, data::TimeFrequency<Cuda, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, uint8_t>, data::TimeFrequency<Cuda, float>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, uint16_t>, data::TimeFrequency<Cuda, float>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, float>, data::TimeFrequency<Cuda, float>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, uint8_t>, data::FrequencyTime<Cuda, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, uint16_t>, data::FrequencyTime<Cuda, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, float>, data::FrequencyTime<Cuda, uint8_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, uint8_t>, data::FrequencyTime<Cuda, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, uint16_t>, data::FrequencyTime<Cuda, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, float>, data::FrequencyTime<Cuda, uint16_t>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, uint8_t>, data::FrequencyTime<Cuda, float>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, uint16_t>, data::FrequencyTime<Cuda, float>>
    , CommonCopyAndTransposeTesterTraits<data::FrequencyTime<Cuda, float>, data::FrequencyTime<Cuda, float>>
    > FTTypes;

INSTANTIATE_TYPED_TEST_SUITE_P(FrequencyTimeTest, CommonCopyAndTransposeTester, FTTypes);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska