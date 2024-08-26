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
#include "cheetah/modules/ddtr/test/TimeFrequencyBufferFactoryTest.h"
#include "cheetah/modules/ddtr/detail/TimeFrequencyBufferFactory.h"
#include "panda/AggregationBufferFiller.h"
#include "panda/test/AggregationBufferFillerTester.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace test {


TimeFrequencyBufferFactoryTest::TimeFrequencyBufferFactoryTest()
    : ::testing::Test()
{
}

void TimeFrequencyBufferFactoryTest::SetUp()
{
}

void TimeFrequencyBufferFactoryTest::TearDown()
{
}


template<typename NumericalRep>
class TimeFrequencyBufferAggregationBufferFillerTestTraits : public ska::panda::test::AggregationBufferFillerTestTraits<panda::AggregationBufferFiller<data::TimeFrequency<Cpu,NumericalRep>, ska::cheetah::modules::ddtr::TimeFrequencyBufferFactory<data::TimeFrequency<Cpu, NumericalRep>>>>
{
        typedef ska::panda::test::AggregationBufferFillerTestTraits<ska::panda::AggregationBufferFiller<data::TimeFrequency<Cpu,NumericalRep>, ska::cheetah::modules::ddtr::TimeFrequencyBufferFactory<data::TimeFrequency<Cpu, NumericalRep>>>> BaseT;

    public:
        typedef typename BaseT::BufferFillerType BufferFillerType;
        typedef typename BaseT::FullBufferHandlerT FullBufferHandlerT;
        typedef typename BaseT::ChunkIndex ChunkIndex;
        typedef typename BaseT::SizeType SizeType;

    public:

        BufferFillerType* get_buffer_filler(FullBufferHandlerT handler, data::DimensionSize<data::Time> number_of_spectra) override
        {
            return new BufferFillerType(handler, number_of_spectra,  data::DimensionSize<data::Frequency>(3U));
        }

};


} // namespace test
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

namespace ska {
namespace panda {
namespace test {

template<typename NumerialRep>
struct ChunkConstructArgs<cheetah::data::TimeFrequency<Cpu, NumerialRep>>
{
    static inline
    std::tuple<cheetah::data::DimensionSize<cheetah::data::Time>, cheetah::data::DimensionSize<cheetah::data::Frequency>> construction_args(cheetah::data::DimensionSize<cheetah::data::Time> size)
    {
        return std::make_tuple(size, cheetah::data::DimensionSize<cheetah::data::Frequency>(3));
    }
};

// use the AggregateBufferFillerTester to test interfaces
typedef ::testing::Types<ska::cheetah::modules::ddtr::test::TimeFrequencyBufferAggregationBufferFillerTestTraits<char>
                        , ska::cheetah::modules::ddtr::test::TimeFrequencyBufferAggregationBufferFillerTestTraits<uint8_t>
                        , ska::cheetah::modules::ddtr::test::TimeFrequencyBufferAggregationBufferFillerTestTraits<uint16_t>
                        , ska::cheetah::modules::ddtr::test::TimeFrequencyBufferAggregationBufferFillerTestTraits<float>
                        , ska::cheetah::modules::ddtr::test::TimeFrequencyBufferAggregationBufferFillerTestTraits<uint32_t>
                        , ska::cheetah::modules::ddtr::test::TimeFrequencyBufferAggregationBufferFillerTestTraits<int>
                        > TimeFrequencyBufferFactoryTypes;

INSTANTIATE_TYPED_TEST_SUITE_P(TimeFrequencyBufferFactoryTests, AggregationBufferFillerTester, TimeFrequencyBufferFactoryTypes);

} // namespace test
} // namespace panda
} // namespace ska
