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
#include "cheetah/io/producers/rcpt_mid/test/BeamFormerPacketTest.h"
#include "cheetah/io/producers/rcpt_mid/BeamFormerPacketInspector.h"

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_mid {
namespace test {


BeamFormerPacketTest::BeamFormerPacketTest()
    : ::testing::Test()
{
}

BeamFormerPacketTest::~BeamFormerPacketTest()
{
}

void BeamFormerPacketTest::SetUp()
{
}

void BeamFormerPacketTest::TearDown()
{
}

/**
 * @brief for mid payload size should be equal to number of elements.
 */
TEST_F(BeamFormerPacketTest, test_number_of_elements)
{
    ASSERT_EQ(BeamFormerPacketMid::data_size(), BeamFormerPacketMid::number_of_elements());
}

/**
 * @brief number of samples estimated from the data size
 */
TEST_F(BeamFormerPacketTest, test_packet_samples)
{
    BeamFormerPacketMid packet;

    ASSERT_EQ(packet.number_of_samples(), packet.data_size()/(4*packet.number_of_channels()));

}

/**
 * @brief test packet count
 */
TEST_F(BeamFormerPacketTest, test_packet_count)
{
    BeamFormerPacketMid packet;

    // min
    packet.packet_count(0);
    ASSERT_EQ(0x0000000000000000U, packet.packet_count());

    packet.packet_count(1);
    ASSERT_EQ(0x0000000000000001U, packet.packet_count());

    // max
    packet.packet_count(0xFFFFFFFFFFFFFFFF);
    ASSERT_EQ(0xFFFFFFFFFFFFFFFFU, packet.packet_count());

    // > 6 bytes
    packet.packet_count(0x0001000000000000);
    ASSERT_EQ(0x0001000000000000U, packet.packet_count());

    // low end range
    for(uint64_t i=2; i<0xFFFF; ++i) {
        packet.packet_count(i);
        ASSERT_EQ(i, packet.packet_count());
    }
}

} // namespace test
} // namespace rcpt_mid
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
