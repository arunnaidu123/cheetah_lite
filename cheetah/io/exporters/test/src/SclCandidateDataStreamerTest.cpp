/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2024 The SKA organisation
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
#include "cheetah/io/exporters/test/SclCandidateDataStreamerTest.h"
#include "cheetah/io/exporters/SclCandidateDataStreamer.h"
#include "cheetah/utils/JulianClock.h"
#include "cheetah/data/Units.h"
#include "panda/test/TestDir.h"

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {
namespace test {

SclCandidateDataStreamerTest::SclCandidateDataStreamerTest()
    : ::testing::Test()
{
}

SclCandidateDataStreamerTest::~SclCandidateDataStreamerTest()
{
}

void SclCandidateDataStreamerTest::SetUp()
{
}

void SclCandidateDataStreamerTest::TearDown()
{
}

TEST_F(SclCandidateDataStreamerTest, test_write)
{
    typedef data::Scl DataType;
    typedef typename DataType::CandidateType CandidateType;
    typedef typename CandidateType::Dm Dm;
    typedef typename CandidateType::MsecTimeType MsecTimeType;
    typedef typename CandidateType::SecPerSecType SecPerSecType;

    // create some candidates
    CandidateType cand1(MsecTimeType(0.01 * boost::units::si::seconds)
                        , SecPerSecType(0.0)
                        , Dm(30.0 * data::parsecs_per_cube_cm)
                        , 1
                        , MsecTimeType(0.001 * boost::units::si::seconds)
                        , 2.0
                       );

    SclCandidateDataStreamerConfig config;

    panda::test::TestDir tmp_dir;
    ASSERT_NO_THROW(tmp_dir.create());
    config.dir(tmp_dir.dir_name());

    {
        // create some data to write
        DataType data;
        // Add some condidates
        for(unsigned i=0; i<3; ++i)
        {
            data.push_back(CandidateType(cand1));
        }
        exporters::SclCandidateDataStreamer writer(config);
        writer << data;
    }
// destructor should be called here, flushing buffers etc
    sync(); // ensure the OS actually writes to disc
    auto it = boost::filesystem::directory_iterator(tmp_dir.path());
    ASSERT_FALSE(it == boost::filesystem::directory_iterator());
    boost::filesystem::path file = *it;
    ASSERT_EQ(file.extension().string(), ".sccl");
}

} // namespace test
} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
