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
#include "cheetah/io/producers/rcpt_low/test/BeamFormerPacketInspectorTest.h"
#include "cheetah/io/producers/rcpt_low/BeamFormerPacketInspector.h"
#include "cheetah/data/TimeFrequency.h"
#include <memory>
#include <memory>

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_low {
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

TEST_F(BeamFormerPacketInspectorTest, test_ignore)
{
    BeamFormerPacketLow packet;
    packet.magic_word(25146554);
    {
        PacketInspectorLow inspector(packet);
        ASSERT_FALSE(inspector.ignore());
    }
    packet.magic_word(25746555);
    {
        PacketInspectorLow inspector(packet);
        ASSERT_TRUE(inspector.ignore());
    }
}

TEST_F(BeamFormerPacketInspectorTest, test_sequence_number)
{
    BeamFormerPacketLow packet;

    packet.packet_count(0);
    packet.first_channel_number(0);
    {
        PacketInspectorLow inspector(packet);
        ASSERT_EQ(0U, inspector.sequence_number());
    }
    packet.packet_count(55);
    packet.first_channel_number(0);
    {

        PacketInspectorLow inspector(packet);
        ASSERT_EQ(47520U, inspector.sequence_number());
    }
}


} // namespace test
} // namespace rcpt_low
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
