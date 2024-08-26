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
#include "cheetah/modules/ddtr/test/RfiExcisionFactoryTest.h"
#include "cheetah/modules/ddtr/RfiExcisionConfig.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/RfimFlaggedData.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace test {


RfiExcisionFactoryTest::RfiExcisionFactoryTest()
    : ::testing::Test()
{
}

RfiExcisionFactoryTest::~RfiExcisionFactoryTest()
{
}

void RfiExcisionFactoryTest::SetUp()
{
}

void RfiExcisionFactoryTest::TearDown()
{
}

TEST_F(RfiExcisionFactoryTest, test_returned_flagged_data_iterator)
{
    std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>> data_ptr(new data::TimeFrequency<Cpu, uint8_t>(data::DimensionSize<data::Time>(1000), data::DimensionSize<data::Frequency>(10)));
    data::RfimFlaggedData<data::TimeFrequency<Cpu, uint8_t>> flagged_data(data::DimensionSize<data::Time>(1000), data::DimensionSize<data::Frequency>(10));
    RfiExcisionConfig config;
    RfiExcisionFactory<data::TimeFrequency<Cpu, uint8_t>> it_factory(config);
    auto it = it_factory.begin(flagged_data);
    RfiExcision<data::TimeFrequency<Cpu, uint8_t>, decltype(flagged_data)> it1(flagged_data, 0, 7.0, 5.0, false);
    ASSERT_TRUE(it1 == it);
    auto it_end = it_factory.end(flagged_data);
    RfiExcision<data::TimeFrequency<Cpu, uint8_t>, decltype(flagged_data)> it1_end(flagged_data, 10000, 7.0, 5.0, false);
    ASSERT_TRUE(it1_end == it_end);
}

TEST_F(RfiExcisionFactoryTest, test_returned_tf_data_iterator)
{
    data::TimeFrequency<Cpu, uint8_t> data(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(1024));
    RfiExcisionConfig config;
    RfiExcisionFactory<decltype(data)> it_factory(config);
    auto it = it_factory.begin(data);
    auto it_end = it_factory.end(data);
    ASSERT_EQ(data.begin(), it);
    ASSERT_EQ(data.end(), it_end);
}

TEST_F(RfiExcisionFactoryTest, test_returned_const_tf_data_iterator)
{
    const data::TimeFrequency<Cpu, uint8_t> data(data::DimensionSize<data::Time>(100),data::DimensionSize<data::Frequency>(1024));
    RfiExcisionConfig config;
    RfiExcisionFactory<decltype(data)> it_factory(config);
    auto it = it_factory.begin(data);
    auto it_end = it_factory.end(data);
    ASSERT_EQ(data.begin(), it);
    ASSERT_EQ(data.end(), it_end);
}

TEST_F(RfiExcisionFactoryTest, test_correct_returned_value)
{
    std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>> data_ptr(new data::TimeFrequency<Cpu, uint8_t>(data::DimensionSize<data::Time>(16), data::DimensionSize<data::Frequency>(8)));
    uint8_t n=0;
    std::generate((*data_ptr).begin(),(*data_ptr).end(),[&](){return ++n;});
    data::RfimFlaggedData<data::TimeFrequency<Cpu, uint8_t>> flagged_data((*data_ptr));
    // Set all flags to false
    flagged_data.rfi_flags().reset(false);
    RfiExcision<data::TimeFrequency<Cpu, uint8_t>, decltype(flagged_data)> it(flagged_data, 0, 7.0, 5.0, false);

    auto data_it = flagged_data.tf_data().begin();

    // Check if we get the correct numerical rep from the full chunk
    for (std::uint32_t ii=0; ii < 127; ++ii)
    {
        ++it;
        ++data_it;
        ASSERT_EQ(*it, *data_it);
    }
}

TEST_F(RfiExcisionFactoryTest, test_end_iterator)
{
    std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>> data_ptr(new data::TimeFrequency<Cpu, uint8_t>(data::DimensionSize<data::Time>(1000), data::DimensionSize<data::Frequency>(10)));
    std::fill((*data_ptr).begin(), (*data_ptr).end(), 1U);
    RfiExcisionConfig config;
    data::RfimFlaggedData<data::TimeFrequency<Cpu, uint8_t>> flagged_data((*data_ptr));
    RfiExcisionFactory<data::TimeFrequency<Cpu, uint8_t>> it_factory(config);
    auto it = it_factory.end(flagged_data);

    RfiExcision<data::TimeFrequency<Cpu, uint8_t>,decltype(flagged_data)> it2(flagged_data, flagged_data.tf_data().data_size(), 7.0, 5.0, false);
    ASSERT_EQ(it,it2);
}

} // namespace test
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
