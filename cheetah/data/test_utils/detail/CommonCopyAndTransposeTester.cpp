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

#include "cheetah/data/TimeFrequency.h"
#include <random>

namespace ska {
namespace cheetah {
namespace data {
namespace test {


template <typename TestTraits>
CommonCopyAndTransposeTester<TestTraits>::CommonCopyAndTransposeTester()
{
}

template <typename TestTraits>
CommonCopyAndTransposeTester<TestTraits>::~CommonCopyAndTransposeTester()
{
}

template <typename TestTraits>
void CommonCopyAndTransposeTester<TestTraits>::SetUp()
{
}

template <typename TestTraits>
void CommonCopyAndTransposeTester<TestTraits>::TearDown()
{
}

template<typename SrcT, typename DstT>
void run_test(std::size_t nchans, std::size_t nspectra)
{
    typedef SrcT SrcType;
    typedef DstT DstType;

    data::DimensionSize<data::Time> number_of_spectra(nspectra);
    data::DimensionSize<data::Frequency> number_of_channels(nchans);

    data::TimeFrequency<Cpu, typename SrcType::NumericalRep> src_host_data(number_of_spectra, number_of_channels);
    srand((unsigned) time(0));
    std::generate(src_host_data.begin(), src_host_data.end(), [&]() { return (rand()%255); });

    SrcType source_data(src_host_data);

    auto f1 =  typename SrcType::FrequencyType(100.0 * boost::units::si::hertz);
    auto f2 =  typename SrcType::FrequencyType(200.0 * boost::units::si::hertz);
    typename utils::ModifiedJulianClock::time_point epoch(utils::julian_day(50000.0));
    auto delta = (f2 - f1)/ (double)(number_of_channels-1);

    source_data.set_channel_frequencies_const_width( f1, delta);
    source_data.start_time(epoch);

    DstType destin_data(source_data);

    data::TimeFrequency<Cpu, typename DstType::NumericalRep> dst_host_data(destin_data);

    for(unsigned i=0; i < source_data.number_of_spectra(); ++i)
    {
        // iterate over channels
        typename data::TimeFrequency<Cpu, typename SrcType::NumericalRep>::Spectra input_spectrum = src_host_data.spectrum(i);
        auto input_channel_it = input_spectrum.begin();
        typename data::TimeFrequency<Cpu, typename DstType::NumericalRep>::Spectra output_spectrum = dst_host_data.spectrum(i);
        auto output_channel_it = output_spectrum.begin();
        while(input_channel_it != input_spectrum.end())
        {
            ASSERT_EQ((typename DstType::NumericalRep)*input_channel_it, *output_channel_it);
            ++input_channel_it;
            ++output_channel_it;
        }
    }
}


/**
* @test test_cornerturn_copy generic tester for all the TF and FT data types.
* @given Input (TF or FT) and output (TF or FT) data types in the testTraits
* @when when the copy output contructor is executed
* @then Then the output is compared with the expected result and reports faliure
*       when there any discrepencies.
*/

TYPED_TEST_P(CommonCopyAndTransposeTester, test_cornerturn_copy)
{
    typedef typename TypeParam::SrcType SrcType;
    typedef typename TypeParam::DstType DstType;

    for(std::size_t s=128; s<1024; s+=128)
    {
        for(std::size_t c=128; c<1024; c+=128)
        {
            run_test<SrcType, DstType>(c,s);
        }
    }

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<std::size_t> nspectra(8, 1024);
    std::uniform_int_distribution<std::size_t> nchans(8, 1024);

    for(std::size_t i=0; i<10; ++i)
    {
        run_test<SrcType, DstType>(nchans(mt), nspectra(mt));
    }
}


REGISTER_TYPED_TEST_SUITE_P(CommonCopyAndTransposeTester, test_cornerturn_copy);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska