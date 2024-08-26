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
#include "cheetah/sigproc/test/SigProcFileStreamTest.h"
#include "cheetah/sigproc/SigProcFileStream.h"
#include "pss/astrotypes/sigproc/FileReader.h"

#include "panda/DataManager.h"
#include "panda/test/gtest.h"

namespace ska {
namespace cheetah {
namespace sigproc {
namespace test {


SigProcFileStreamTest::SigProcFileStreamTest()
    : ::testing::Test()
{
}

SigProcFileStreamTest::~SigProcFileStreamTest()
{
}

void SigProcFileStreamTest::SetUp()
{
}

void SigProcFileStreamTest::TearDown()
{
}

TEST_F(SigProcFileStreamTest, test_empty_file_name)
{
    Config config;
    SigProcFileStream stream(config);
    panda::DataManager<SigProcFileStream> chunk_manager(stream);
    ASSERT_TRUE(stream.process()); // end of stream
}

TEST_F(SigProcFileStreamTest, test_single_valid_file_defined)
{
    Config config;
    config.set_sigproc_files(panda::test::test_file("valid_sigproc.sigproc"));
    SigProcFileStream stream(config);
    panda::DataManager<SigProcFileStream> chunk_manager(stream);
    ASSERT_FALSE(stream.process());

    std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data_tuple=chunk_manager.next();
    data::TimeFrequency<Cpu, uint8_t>& data = *(std::get<0>(data_tuple));
    ASSERT_EQ(250368U, data.number_of_spectra());
    ASSERT_EQ(std::size_t(2), data.number_of_channels());
}

TEST_F(SigProcFileStreamTest, test_single_invalid_file_defined)
{
    Config config;
    config.set_sigproc_files(panda::test::test_file("invalid_sigproc.sigproc"));
    SigProcFileStream stream(config);
    panda::DataManager<SigProcFileStream> chunk_manager(stream);
    ASSERT_THROW(stream.process(), panda::Error);
    ASSERT_TRUE(stream.process()); // end of stream
}

TEST_F(SigProcFileStreamTest, test_invalid_file_followed_by_valid_file)
{
    Config config;
    config.set_sigproc_files(std::vector<std::string>({panda::test::test_file("invalid_sigproc.sigproc"), panda::test::test_file("valid_sigproc.sigproc") }));
    SigProcFileStream stream(config);
    panda::DataManager<SigProcFileStream> chunk_manager(stream);
    ASSERT_THROW(stream.process(), panda::Error);
    ASSERT_FALSE(stream.process()); // valid data
}

TEST_F(SigProcFileStreamTest, test_chunk_samples_from_config_preset)
{
    // prep test files
    std::string sigproc_file { panda::test::test_file("valid_sigproc.sigproc") };
    pss::astrotypes::sigproc::FileReader<> sfr(sigproc_file);
    std::size_t number_of_spectra=sfr.number_of_data_points()/sfr.header().number_of_channels();

    for(unsigned i=1; i<7; ++i) {
        SCOPED_TRACE(i);
        Config config;
        config.set_sigproc_files(sigproc_file);
        config.set_chunk_samples(i);

        SigProcFileStream stream(config);
        panda::DataManager<SigProcFileStream> chunk_manager(stream);
        ASSERT_FALSE(stream.process()) << i;
        std::size_t number_of_chunks = number_of_spectra/i;
        for(unsigned chunk=0; chunk<number_of_chunks; ++chunk) {
            std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data = chunk_manager.next();
            ASSERT_EQ(i, std::get<0>(data)->number_of_spectra());
            ASSERT_EQ(std::size_t(2), std::get<0>(data)->number_of_channels());
            ASSERT_EQ( std::get<0>(data)->end() - std::get<0>(data)->begin(), i * std::get<0>(data)->number_of_channels());
        }
        // check chunks that do not match the exact length of the file (should be resized to fit)
        std::size_t remainder = number_of_spectra - number_of_chunks * i;
        if(remainder > 0) {
            std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data = chunk_manager.next();
            ASSERT_EQ(remainder, (std::size_t)std::get<0>(data)->number_of_spectra());
            ASSERT_EQ(std::size_t(2), std::get<0>(data)->number_of_channels());
            ASSERT_NE(0.0, *std::get<0>(data)->begin()); // check we get some data
        }
    }
}

} // namespace test
} // namespace sigproc
} // namespace cheetah
} // namespace ska
