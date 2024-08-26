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
#include "cheetah/io/producers/rcpt_low/test/BeamFormerDataTraitsTest.h"
#include "cheetah/io/producers/rcpt_low/BeamFormerPacketInspector.h"
#include "cheetah/io/producers/rcpt_low/BeamFormerDataTraits.h"
#include "panda/concepts/ChunkerContextDataTraitsConcept.h"
#include <vector>


namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_low {
namespace test {


BeamFormerDataTraitsTest::BeamFormerDataTraitsTest()
    : ::testing::Test()
{
}

BeamFormerDataTraitsTest::~BeamFormerDataTraitsTest()
{
}

void BeamFormerDataTraitsTest::SetUp()
{
}

void BeamFormerDataTraitsTest::TearDown()
{
}

TEST_F(BeamFormerDataTraitsTest, test_packets_per_chunk)
{
    BOOST_CONCEPT_ASSERT((panda::ChunkerContextDataTraitsConcept<BeamFormerDataTraits<PssLowTraits>>));

    typedef BeamFormerDataTraitsLow::DataType DataType;
    //std::size_t samples_per_packet = BeamFormerDataTraits<int8_t,128,8>::PacketInspector::Packet::number_of_samples();
    BeamFormerDataTraitsLow traits;
    {
        // number_of_channels == payload size
        data::DimensionSize<data::Frequency> number_of_channels(9);
        data::DimensionSize<data::Time> number_of_time_samples(128);

        DataType data(number_of_time_samples, number_of_channels);
        unsigned packets_per_chunk = (4*traits.chunk_size(data))/(traits.data_size());
        ASSERT_EQ(1, packets_per_chunk);
    }
}

template<typename T>
struct TestContext {
    public:
        TestContext()
            : _data(new data::FrequencyTime<Cpu, T>(data::DimensionSize<data::Time>(128), data::DimensionSize<data::Frequency>(9)))
        {}

        std::size_t offset() const { return 0; }
        std::size_t packet_offset() const { return 0; }
        data::FrequencyTime<Cpu, T>& chunk() { return *_data; }
        std::size_t size() const { return 128*9; }

        uint64_t sequence_number(BeamFormerDataTraitsLow::PacketInspector const& packet) const { return packet.sequence_number();}

    private:
        std::shared_ptr<data::FrequencyTime<Cpu, T>> _data;
        std::shared_ptr<TestContext> _next;
};


template<typename T>
std::ostream& operator<<(std::ostream& os, TestContext<T> const&) { return os; }

TEST_F(BeamFormerDataTraitsTest, test_deserialise)
{
    // basic compile test
    BeamFormerPacketLow packet;
    packet.packet_count(0);
    packet.packet_type(BeamFormerPacketLow::PacketType::PssLow);
    BeamFormerDataTraitsLow dt;
    typedef BeamFormerDataTraitsLow::DataType::value_type ValueType;
    TestContext<ValueType> context;
    std::fill(packet.begin(), packet.end(), 1);
    packet.set_unit_weights();
    dt.deserialise_packet(context, packet);
    data::FrequencyTime<Cpu, ValueType> chunk = context.chunk();
    for(uint32_t i=0; i<chunk.number_of_channels()*chunk.number_of_spectra(); ++i)
    {
        ASSERT_EQ(4, *(chunk.begin()+i));
    }
}


} // namespace test
} // namespace rcpt_low
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
