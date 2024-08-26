/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#include "cheetah/io/producers/rcpt_mid/test/BeamFormerDataTraitsTest.h"
#include "cheetah/io/producers/rcpt_mid/BeamFormerPacketInspector.h"
#include "cheetah/io/producers/rcpt_mid/BeamFormerDataTraits.h"
#include "panda/concepts/ChunkerContextDataTraitsConcept.h"
#include <vector>


namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_mid {
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
    BOOST_CONCEPT_ASSERT((panda::ChunkerContextDataTraitsConcept<BeamFormerDataTraits<PssMidTraits>>));

    typedef BeamFormerDataTraitsMid::DataType DataType;
    //std::size_t samples_per_packet = BeamFormerDataTraits<int8_t,128,8>::PacketInspector::Packet::number_of_samples();
    BeamFormerDataTraitsMid traits;
    {
        // number_of_channels == payload size
        data::DimensionSize<data::Frequency> number_of_channels(185);
        data::DimensionSize<data::Time> number_of_time_samples(8);

        DataType data(number_of_time_samples, number_of_channels);
        unsigned packets_per_chunk = (4*traits.chunk_size(data))/(traits.data_size());
        ASSERT_EQ(1, packets_per_chunk);
    }
}

struct TestContext {
    public:
        TestContext()
            : _data(new data::FrequencyTime<Cpu, uint8_t>(data::DimensionSize<data::Time>(8)
                                                        , data::DimensionSize<data::Frequency>(3700)))
            , _number_of_channels_per_packet(8)
            , _number_of_samples_per_packet(185)
        {
        }

        std::size_t offset() const
        {
            return 0;
        }

        std::size_t packet_offset() const
        {
            return 0;
        }

        data::FrequencyTime<Cpu, uint8_t>& chunk()
        {
            return *_data;
        }

        TestContext& next()
        {
            _next.reset(new TestContext());
            return *_next;
        }

        std::size_t size() const
        {
            return _number_of_channels_per_packet*_number_of_samples_per_packet;
        }

        uint64_t sequence_number(BeamFormerDataTraits<PssMidTraits>::PacketInspector const& packet) const
        {
            return packet.sequence_number();
        }

    private:
        std::shared_ptr<data::FrequencyTime<Cpu, uint8_t>> _data;
        std::shared_ptr<TestContext> _next;
        std::size_t _number_of_channels_per_packet;
        std::size_t _number_of_samples_per_packet;
};

std::ostream& operator<<(std::ostream& os, TestContext const&)
{
    return os;
}


TEST_F(BeamFormerDataTraitsTest, test_deserialise)
{
    // basic compile test
    BeamFormerPacketMid packet;
    packet.packet_count(0);
    packet.packet_type(BeamFormerPacketMid::PacketType::PssMid);

    srand((unsigned) time(0));
    std::generate(packet.begin(), packet.end(), [&]() { return rand()%255; });


    BeamFormerDataTraitsMid dt;
    TestContext context;
    dt.deserialise_packet(context, packet);
    auto it = packet.begin();
    auto chunk_it = context.chunk().begin();
    for(unsigned i=0; i<packet.number_of_channels()*packet.number_of_samples(); i++)
    {
        ASSERT_EQ((unsigned)(*chunk_it), (unsigned)(*it));
        ++it;
        ++chunk_it;
    }

    for(unsigned i=packet.number_of_channels()*packet.number_of_samples(); i<context.size(); i++)
    {
        ASSERT_EQ((unsigned)(*chunk_it), 0U);
        ++chunk_it;
    }

}


} // namespace test
} // namespace rcpt_mid
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
