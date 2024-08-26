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
#include "cheetah/data/Ocld.h"
#include "cheetah/io/exporters/test/OcldFileStreamerTest.h"
#include "cheetah/io/exporters/OcldFileStreamer.h"
#include "panda/test/TestDir.h"


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {
namespace test {

template<typename TypeParam>
OcldFileStreamerTest<TypeParam>::OcldFileStreamerTest()
    : ::testing::Test()
{
}

template<typename TypeParam>
OcldFileStreamerTest<TypeParam>::~OcldFileStreamerTest()
{
}

template<typename TypeParam>
void OcldFileStreamerTest<TypeParam>::SetUp()
{
}

template<typename TypeParam>
void OcldFileStreamerTest<TypeParam>::TearDown()
{
}

TYPED_TEST(OcldFileStreamerTest, test_write_out)
{
    panda::test::TestDir tmp_dir;
    ASSERT_NO_THROW(tmp_dir.create());

    typedef typename data::OptimisedCandidate<TypeParam>::CandidateType Candidate;

    typename Candidate::TimeType pulse_period(100.0 * boost::units::si::milli * boost::units::si::seconds);
    typename Candidate::SecPerSecType pulse_pdot(0.0);
    typename Candidate::Dm dm(40.0 * data::parsec_per_cube_cm);
    typename Candidate::MsecTimeType pulse_width(1.0 * boost::units::si::milli * boost::units::si::seconds);

    data::OptimisedCandidate<TypeParam>* candidate = new data::OptimisedCandidate<TypeParam>();
    candidate->metadata().period(pulse_period);
    candidate->metadata().pdot(pulse_pdot);
    candidate->metadata().dm(dm);
    candidate->metadata().width(pulse_width);
    candidate->metadata().sigma(1.0);

    data::Ocld<TypeParam> ocld;
    ocld.add(*candidate);
    OcldFileStreamerConfig config;
    config.dir(tmp_dir.dir_name());
    OcldFileStreamer<TypeParam> writer(config);
    writer << ocld;
    sync();
    auto it = boost::filesystem::directory_iterator(tmp_dir.path());
    boost::filesystem::path file = *it;
    ASSERT_EQ(file.extension().string(), ".ocld");
    ASSERT_EQ(1U, std::distance(it, boost::filesystem::directory_iterator()));
}

} // namespace test
} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
