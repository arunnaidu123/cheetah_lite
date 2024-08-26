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
#include "cheetah/sigproc/test/SigProcWriterTest.h"
#include "cheetah/sigproc/SigProcWriter.h"
#include "cheetah/sigproc/SigProcFileStream.h"
#include "cheetah/utils/JulianClock.h"
#include "pss/astrotypes/sigproc/FileReader.h"
#include "panda/test/TestDir.h"
#include <climits>
#include <algorithm>
#include "unistd.h"


namespace ska {
namespace cheetah {
namespace sigproc {
namespace test {


SigProcWriterTest::SigProcWriterTest()
    : ::testing::Test()
{
}

SigProcWriterTest::~SigProcWriterTest()
{
}

void SigProcWriterTest::SetUp()
{
}

void SigProcWriterTest::TearDown()
{
}

TEST_F(SigProcWriterTest, test_write_read)
{
    data::DimensionSize<data::Time> samples(20);
    pss::astrotypes::units::Quantity<pss::astrotypes::units::Seconds, double> sample_interval(1.0 * pss::astrotypes::units::milliseconds);
    // create a tmp directoy name
    for(data::DimensionSize<data::Frequency> i(1); i<data::DimensionSize<data::Frequency>(9046); i+=data::DimensionSize<data::Frequency>(1024)) { // try different channel numbers
        panda::test::TestDir tmp_dir;
        ASSERT_NO_THROW(tmp_dir.create());
        {
            typename utils::JulianClock::time_point start_time(utils::julian_day(2458179.500000));
            data::TimeFrequency<Cpu, uint8_t> tf1(samples, i);
            tf1.start_time(static_cast<typename data::TimeFrequency<Cpu, uint8_t>::TimePointType>(start_time));
            tf1.sample_interval(sample_interval);
            data::TimeFrequency<Cpu, uint8_t> tf2(samples, i);
            tf2.start_time(tf1.end_time() + tf1.sample_interval());
            tf2.sample_interval(sample_interval);
            SigProcWriter<> writer(tmp_dir.dir_name());
            writer << tf1;
            writer << tf2;
        } // should flush buffers to OS on leaving scope
        sync(); // ensure the OS actually writes to disc
        auto it = boost::filesystem::directory_iterator(tmp_dir.path());
        ASSERT_FALSE(it == boost::filesystem::directory_iterator());
        Config config;
        ASSERT_TRUE(boost::filesystem::exists(it->path()));
        config.set_sigproc_files(it->path().native());
        config.set_chunk_samples(3*samples); // should be reduced

        std::vector<boost::filesystem::path> files;
        std::copy(it, boost::filesystem::directory_iterator(), std::back_inserter(files));
        std::sort(files.begin(), files.end());
        ASSERT_EQ(files.size(), 1);

        boost::filesystem::path file = files[0];
        ASSERT_EQ(file.extension().string(), ".fil");
        ASSERT_EQ(file.stem().string(), std::string("2018_03_02_00:00:00"));

        SigProcFileStream stream(config);
        panda::DataManager<SigProcFileStream> chunk_manager(stream);
        ASSERT_TRUE(stream.process()) << "channels=" << i; // expect to of tried opening a new file (and fail) to find the missing data
        std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data = chunk_manager.next();
        ASSERT_EQ(static_cast<std::size_t>(i), std::get<0>(data)->number_of_channels());
        ASSERT_EQ(2*samples, std::get<0>(data)->number_of_spectra()) << "channel=" << i;
    }
}

template<typename NumRep>
void write_truncate_test()
{
    data::DimensionSize<data::Time> samples(20);
    data::DimensionSize<data::Time> samples_2(20);
    data::DimensionSize<data::Frequency> freq(100);
    pss::astrotypes::units::Quantity<pss::astrotypes::units::Seconds, double> sample_interval(1.0 * pss::astrotypes::units::milliseconds);
    typename utils::JulianClock::time_point start_time(utils::julian_day(2458179.500000));

    panda::test::TestDir tmp_dir;
    ASSERT_NO_THROW(tmp_dir.create());

    data::TimeFrequency<Cpu, NumRep> tf1(samples, freq);
    tf1.start_time(static_cast<typename data::TimeFrequency<Cpu, NumRep>::TimePointType>(start_time));
    tf1.sample_interval(sample_interval);
    data::TimeFrequency<Cpu, NumRep> tf2(samples_2, freq);
    tf2.start_time(tf1.end_time() + tf1.sample_interval());
    tf2.sample_interval(sample_interval);

    // Case: chunk > max count
    WriterConfig config;
    config.max_count(samples-1);
    config.dir(tmp_dir.dir_name());
    SigProcWriter<> writer(config);
            writer << tf1;
            writer << tf2;
    writer.flush();
    sync(); // ensure the OS actually writes to disc

    auto it = boost::filesystem::directory_iterator(tmp_dir.path());
    std::vector<boost::filesystem::path> files;
    std::copy(it, boost::filesystem::directory_iterator(), std::back_inserter(files));
    std::sort(files.begin(), files.end());
    ASSERT_EQ(files.size(), 3);

    boost::filesystem::path file = files[0];
    std::cout << "examining file: " << file.string() << "\n";
    pss::astrotypes::sigproc::FileReader<> reader(file.string());
    pss::astrotypes::sigproc::Header header1 = reader.header();
    ASSERT_EQ((std::size_t)reader.dimension<data::Time>(), config.max_count());

    // examine second file
    boost::filesystem::path file2 = files[1];
    std::cout << "examining file: " << file2.string() << "\n";
    reader.open(file2.string());
    pss::astrotypes::sigproc::Header header2 = reader.header();

    ASSERT_EQ(header1, header2);
    ASSERT_EQ((std::size_t)reader.dimension<data::Time>(), config.max_count());
    // examine second file
    boost::filesystem::path file3 = files[2];
    reader.open(file3.string());
    pss::astrotypes::sigproc::Header header3 = reader.header();

    ASSERT_EQ(header1, header3);
    ASSERT_EQ((std::size_t)reader.dimension<data::Time>(), 2);

}

TEST_F(SigProcWriterTest, test_write_truncate_uint8_t)
{
    write_truncate_test<uint8_t>();
}

TEST_F(SigProcWriterTest, test_write_truncate_uint16_t)
{
    write_truncate_test<uint16_t>();
}

} // namespace test
} // namespace sigproc
} // namespace cheetah
} // namespace ska
