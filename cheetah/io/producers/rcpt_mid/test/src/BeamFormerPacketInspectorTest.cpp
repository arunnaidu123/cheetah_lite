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
#include "cheetah/io/producers/rcpt_mid/test/BeamFormerPacketInspectorTest.h"
#include "cheetah/io/producers/rcpt_mid/BeamFormerPacketInspector.h"

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_mid {
namespace test {


BeamFormerPacketInspectorTest::BeamFormerPacketInspectorTest()
    : ::testing::Test()
{
}

BeamFormerPacketInspectorTest::~BeamFormerPacketInspectorTest()
{
}

void BeamFormerPacketInspectorTest::SetUp()
{
}

void BeamFormerPacketInspectorTest::TearDown()
{
}

/**
 * @brief sanity check to make sure packet is not corrupted
 */
TEST_F(BeamFormerPacketInspectorTest, test_ignore)
{
    BeamFormerPacketMid packet;
    packet.magic_word(25146554);
    {
        PacketInspectorMid inspector(packet);
        ASSERT_FALSE(inspector.ignore());
    }
    packet.magic_word(25746555);
    {
        PacketInspectorMid inspector(packet);
        ASSERT_TRUE(inspector.ignore());
    }
}

/**
 * @brief check if the sequence number is as expected
 */
TEST_F(BeamFormerPacketInspectorTest, test_sequence_number)
{
    BeamFormerPacketMid packet;

    packet.packet_count(0);
    packet.first_channel_number(0);
    {
        PacketInspectorMid inspector(packet);
        ASSERT_EQ(0U, inspector.sequence_number());
    }
    packet.packet_count(55);
    packet.first_channel_number(185);
    {

        PacketInspectorMid inspector(packet);
        // sequence number = 55*3700/185 + 1
        ASSERT_EQ(1101U, inspector.sequence_number());
    }
}

} // namespace test
} // namespace rcpt_mid
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
