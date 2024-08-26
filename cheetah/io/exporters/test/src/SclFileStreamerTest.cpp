/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 The SKA organisation
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
#include "cheetah/io/exporters/test/SclFileStreamerTest.h"
#include "cheetah/io/exporters/SclFileStreamer.h"
#include "panda/test/TestDir.h"
#include <climits>
#include <unistd.h>


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {
namespace test {


SclFileStreamerTest::SclFileStreamerTest()
    : ::testing::Test()
{
}

SclFileStreamerTest::~SclFileStreamerTest()
{
}

void SclFileStreamerTest::SetUp()
{
}

void SclFileStreamerTest::TearDown()
{
}

TEST_F(SclFileStreamerTest, test_write_read)
{
    data::Scl d1;
    panda::test::TestDir tmp_dir;
    ASSERT_NO_THROW(tmp_dir.create());
    //set the candidate DM
    typename data::Scl::CandidateType::Dm dm(12.0 * data::parsecs_per_cube_cm);
    //set the candidate width
    data::Scl::CandidateType::MsecTimeType width(0.001 * boost::units::si::seconds);
    //set the candidate period
    data::Scl::CandidateType::MsecTimeType period(2.0 * boost::units::si::seconds);
    //set the candidate period derivative
    data::Scl::CandidateType::SecPerSecType pdot(1e-4);
    //set the candidate harmonic number
    uint8_t harmonic = 1;
    //set the candidate S/N
    float sigma = 20.0;

    for (auto idx=0; idx<10; ++idx)
    {
        data::Scl::CandidateType candidate(period, pdot, dm, harmonic, width, sigma, idx);
        d1.push_back(candidate);
    }

    SclFileStreamerConfig config;
    config.dir(tmp_dir.dir_name());
    SclFileStreamer writer(config);
    writer << d1;
    sync(); // ensure the OS actually writes to disc
    auto it = boost::filesystem::directory_iterator(tmp_dir.path());

    //To check if directory created exists in the necessary path
    ASSERT_FALSE(it == boost::filesystem::directory_iterator());
    ASSERT_TRUE(boost::filesystem::exists(it->path()));

}

} // namespace test
} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
