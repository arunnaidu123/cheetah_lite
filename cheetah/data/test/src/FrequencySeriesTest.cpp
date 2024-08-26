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
#include "cheetah/data/test_utils/FrequencySeriesTester.h"
#include "cheetah/utils/Architectures.h"
#include "cheetah/data/FrequencySeries.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

typedef ::testing::Types<
    FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cpu, float>, FrequencySeries<cheetah::Cpu, float>>,
    FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cpu, float>, FrequencySeries<cheetah::Cpu, double>>,
    FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cpu, double>, FrequencySeries<cheetah::Cpu, float>>,
    FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cpu, double>, FrequencySeries<cheetah::Cpu, double>>,
    FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cpu, std::complex<float>>, FrequencySeries<cheetah::Cpu, std::complex<float>>>,
    FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cpu, std::complex<float>>, FrequencySeries<cheetah::Cpu, std::complex<double>>>,
    FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cpu, std::complex<double>>, FrequencySeries<cheetah::Cpu, std::complex<float>>>,
    FrequencySeriesTesterTraits<FrequencySeries<cheetah::Cpu, std::complex<double>>, FrequencySeries<cheetah::Cpu, std::complex<double>>>
    > FrequencySeriesTesterTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(FrequencySeries, FrequencySeriesTester, FrequencySeriesTesterTypes);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
