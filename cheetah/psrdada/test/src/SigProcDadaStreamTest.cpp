/*
 ** The MIT License (MIT)
 **
 ** Copyright (c) 2016 The SKA organisation
 **
 ** Permission is hereby granted, free of charge, to any person obtaining a copy
 ** of this software and associated documentation files (the "Software"), to deal
 ** in the Software without restriction, including without limitation the rights
 ** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 ** copies of the Software, and to permit persons to whom the Software is
 ** furnished to do so, subject to the following conditions:
 **
 ** The above copyright notice and this permission notice shall be included in all
 ** copies or substantial portions of the Software.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 ** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 ** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 ** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 ** SOFTWARE.
 **/

#include "cheetah/psrdada/DadaWriteClient.h"
#include "cheetah/sigproc/SigProcHeader.h"
#include "cheetah/generators/GaussianNoise.h"
#include "cheetah/generators/GaussianNoiseConfig.h"
#include "cheetah/psrdada/DadaReadClient.h"
#include "cheetah/psrdada/SigProcDadaStream.h"
#include "cheetah/psrdada/test/SigProcDadaStreamTest.h"
#include "cheetah/psrdada/test_utils/TestDadaDB.h"
#include "panda/DataManager.h"
#include "panda/test/gtest.h"
#include <sstream>
#include <fstream>
#include <condition_variable>

namespace ska {
namespace cheetah {
namespace psrdada {
namespace test {

SigProcDadaStreamTest::SigProcDadaStreamTest()
    : ::testing::Test()
{
}

SigProcDadaStreamTest::~SigProcDadaStreamTest()
{
}

void SigProcDadaStreamTest::SetUp()
{
}

void SigProcDadaStreamTest::TearDown()
{
}

namespace detail {


class SigProcDadaWriter {
    public:
        SigProcDadaWriter(key_t key)
            : _key(key)
        {
        }

        template<typename Arch, typename NumericalT>
        SigProcDadaWriter& operator<<(data::TimeFrequency<Arch, NumericalT> const& tf)
        {
            sigproc::SigProcHeader h;
            h.number_of_channels(tf.number_of_channels());
            h.number_of_bits(sizeof(typename data::TimeFrequency<Arch, NumericalT>::DataType)*8);
            h.start_time(tf.start_time());
            h.sample_interval(tf.sample_interval());
            if( _header != h )
            {
                h.start_time(_header.start_time() + std::chrono::duration<double>(_header.sample_interval().value() * (double)tf.number_of_spectra()));
                _header = h;
                if(_writer)
                {
                    _writer->new_sequence();
                }
                else
                {
                   _writer.reset(new DadaWriteClient(_key, [this](std::ostream& out) {_header.write(out);}));
                }
            }
            auto it = tf.begin();
            _writer->write(it,tf.end());
            _header.start_time(_header.start_time() + std::chrono::duration<double>(_header.sample_interval().value() * (double)tf.number_of_spectra()));
            return *this;
        }

        void new_sequence()
        {
            _writer->new_sequence();
        }

        void release_block()
        {
            _writer->write_eod();
        }

    private:
        key_t _key;
        sigproc::SigProcHeader _header;
        std::unique_ptr<DadaWriteClient> _writer;
};


// Generating some test data

data::TimeFrequency<Cpu, uint8_t> test_data(std::size_t number_of_samples, std::uint32_t number_of_channels)
{
    cheetah::data::TimeFrequency<Cpu, uint8_t> data_chunk;
    static generators::GaussianNoiseConfig config;
    static generators::GaussianNoise<cheetah::data::TimeFrequency<Cpu, uint8_t>> generator(config);
    data_chunk.resize(data::DimensionSize<data::Time>(number_of_samples),data::DimensionSize<data::Frequency>(number_of_channels));
    generator.next(data_chunk);
    data::TimeFrequency<Cpu, uint8_t>::TimeType interval(1.0 * boost::units::si::milli * boost::units::si::seconds);
    data_chunk.sample_interval(interval);
    return data_chunk;
}

data::TimeFrequency<Cpu, uint8_t> test_data(std::size_t number_of_samples, std::uint32_t number_of_channels, int offset)
{
    cheetah::data::TimeFrequency<Cpu, uint8_t> data_chunk;
    static generators::GaussianNoiseConfig config;
    static generators::GaussianNoise<cheetah::data::TimeFrequency<Cpu, uint8_t>> generator(config);
    data_chunk.resize(data::DimensionSize<data::Time>(number_of_samples),data::DimensionSize<data::Frequency>(number_of_channels));
    generator.next(data_chunk);
    data::TimeFrequency<Cpu, uint8_t>::TimeType interval(1.0 * boost::units::si::milli * boost::units::si::seconds);
    std::chrono::milliseconds off(offset);
    data::TimeFrequency<Cpu, uint8_t>::TimePointType offstart(off);
    data_chunk.start_time(offstart);
    data_chunk.sample_interval(interval);
    return data_chunk;
}
} // namespace detail

TEST_F(SigProcDadaStreamTest, test_construction_ordering)
{   // Construct a reader before a writer to make sure that it does not block the main thread

    Config config;
    test_utils::TestDadaDB test_db( 5, 409600, 10, 4096);
    test_db.create();
    config.number_of_samples(10239);
    config.dada_key(test_db.key());
    SigProcDadaStream stream(config);
    detail::SigProcDadaWriter writer(test_db.key());

}

TEST_F(SigProcDadaStreamTest, test_chunk_smaller_than_dada_stream)
{
    // Read in from a stream where the chunk size < the seqeunce size
    // expect the next call will be filled from where the previous chunk left off
    // with correct timestamps set etc.

    Config config;
    test_utils::TestDadaDB test_db( 5, 409600, 10, 4096);
    test_db.create();


    // Write data to the DADA buffer
    //detail::SigProcDadaWriter writer(test_db.key());
    auto data = detail::test_data(20480, 2);
    detail::SigProcDadaWriter writer(test_db.key());
    writer << data;
    writer.new_sequence();
    writer << data;
    writer.new_sequence();

    // Read the data
    config.number_of_samples(10239);
    config.dada_key(test_db.key());
    SigProcDadaStream stream(config);
    panda::DataManager<SigProcDadaStream> chunk_manager(stream);
    ASSERT_FALSE(stream.process());
    std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data_tuple = chunk_manager.next();
    data::TimeFrequency<Cpu, uint8_t>& read_data = *(std::get<0>(data_tuple));

    // first Data verification
    ASSERT_EQ(10239U, read_data.number_of_spectra());
    ASSERT_EQ(std::size_t(2), read_data.number_of_channels());
    auto it = data.begin();
    auto rit = read_data.begin();
    std::size_t total_number_of_samples = read_data.number_of_channels() * read_data.number_of_spectra();
    for (unsigned ii=0; ii<total_number_of_samples; ii++)
    {
        ASSERT_EQ(rit[ii], it[ii]);
    }

    // Second chunk verification
    std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data_tuple1 = chunk_manager.next();
    data::TimeFrequency<Cpu, uint8_t>& read_data1 = *(std::get<0>(data_tuple1));
    ASSERT_EQ(10239U, read_data1.number_of_spectra());
    ASSERT_EQ(std::size_t(2),read_data1.number_of_channels());
    it = data.begin();
    rit = read_data1.begin();
    for (unsigned ii=0; ii<total_number_of_samples; ii++)
    {
        ASSERT_EQ(rit[ii], it[ii+total_number_of_samples]);
    }

}


TEST_F(SigProcDadaStreamTest, test_chunk_with_eod_no_header)
{
    // Read in from a stream where the chunk size < the seqeunce size
    // expect the next call will be filled from where the previous chunk left off
    // expect the stream to exit cleanly when no header is written
    // to the new sequence after the eod

    Config config;
    test_utils::TestDadaDB test_db( 5, 409600, 10, 4096);
    test_db.create();

    // Write data to the DADA buffer
    detail::SigProcDadaWriter writer(test_db.key());
    auto data = detail::test_data(20480,2);
    writer << data;
    writer.release_block();

    // Read the data
    config.number_of_samples(10239);
    config.dada_key(test_db.key());
    SigProcDadaStream stream(config);
    ASSERT_FALSE(stream.process());
    panda::DataManager<SigProcDadaStream> chunk_manager(stream);
    std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t >>> data_tuple1 = chunk_manager.next();
    data::TimeFrequency<Cpu, uint8_t>& read_data1 = *(std::get<0>(data_tuple1));
    ASSERT_EQ(10239U, read_data1.number_of_spectra());
    ASSERT_EQ(std::size_t(2),read_data1.number_of_channels());
    auto it = data.begin();
    auto rit = read_data1.begin();
    for (unsigned ii=0; ii<20478; ii++)
    {
        ASSERT_EQ(rit[ii], it[ii]);
    }

}

TEST_F(SigProcDadaStreamTest, test_multisequence_dada_stream_inconsistent_headers)
{
    // Read in from a stream where the data available is less than
    // the expected number of samples in the data block to be filled.
    // Expect:
    //   At the exhaustion of the data stream, to open up a new sequence and continue
    //   reading from there.
    //   The test needs to verify correct behaviour when the new sequence header
    //   data is inconsistent with the current sequence header data
    //   e.g different nbits, number of channels

    Config config;
    test_utils::TestDadaDB test_db( 10, 409600, 10, 4096);
    test_db.create();

    // Generate test data and write to the dada buffer
    auto data = detail::test_data(10240, 2);
    auto data2 = detail::test_data(10000, 4);
    detail::SigProcDadaWriter writer(test_db.key());
    writer << data;
    writer << data2;
    writer.new_sequence();
    writer << data2;
    writer.new_sequence();

    // Setup the sigprocdada stream
    config.number_of_samples(10241);
    config.dada_key(test_db.key());
    SigProcDadaStream stream(config);
    panda::DataManager<SigProcDadaStream> chunk_manager(stream);
    ASSERT_FALSE(stream.process());
    std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data_tuple = chunk_manager.next();
    data::TimeFrequency<Cpu, uint8_t>& read_data1 = *(std::get<0>(data_tuple));
    ASSERT_EQ(10240U, read_data1.number_of_spectra());
    ASSERT_EQ(std::size_t(2), read_data1.number_of_channels());

    auto it = data.begin();
    auto rit = read_data1.begin();
    // check first data chunk received is as sent
    for (unsigned ii=0; ii<20480; ++ii)
    {
        ASSERT_EQ((rit[ii]), (it[ii]));
    }
    // Parsing data with inconsistent header
    std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data_tuple2 = chunk_manager.next();
    data::TimeFrequency<Cpu, uint8_t>& read_data2 = *(std::get<0>(data_tuple2));
    ASSERT_EQ(10241U, read_data2.number_of_spectra());
    ASSERT_EQ(std::size_t(4), read_data2.number_of_channels());
    rit = read_data2.begin();
    it = data2.begin();
    for (unsigned ii=0; ii<20482; ++ii)
    {
        ASSERT_EQ(rit[ii], it[ii]);
    }
}

TEST_F(SigProcDadaStreamTest, test_multisequence_dada_stream_consistent_headers)
{
    // Read in from a stream where the data available is less than
    // the expected number of samples in in the data block to be filled.
    // Expect:
    //   At the exhaustion of the data stream, to open up a new sequence and continue
    //   reading from there.
    //   The test needs to verify correct behaviour when
    //   the new sequence header data is consistent with the current data

    test_utils::TestDadaDB test_db( 10, 409600, 10, 4096);
    test_db.create();

    Config config;

    std::size_t chunk_time_samples=10241;
    auto test_data = detail::test_data(10240,2);
    detail::SigProcDadaWriter writer(test_db.key());
    writer << test_data;
    writer.new_sequence();
    writer << test_data;
    writer.new_sequence();
    writer << test_data;
    writer.new_sequence();
    writer << test_data;
    writer.new_sequence();
    writer << test_data;
    writer.new_sequence();

    // read the dada buffer data as a stream
    config.number_of_samples(chunk_time_samples);
    config.dada_key(test_db.key());
    SigProcDadaStream stream(config);
    panda::DataManager<SigProcDadaStream> chunk_manager(stream);
    ASSERT_FALSE(stream.process());
    std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data_tuple = chunk_manager.next();
    data::TimeFrequency<Cpu, uint8_t>& read_data = *(std::get<0>(data_tuple));
    ASSERT_EQ(10241U, read_data.number_of_spectra());
    auto it  = test_data.begin();
    auto rit = read_data.begin();
    for(std::size_t ii=0; ii<20480; ++ii)
    {
        ASSERT_EQ(rit[ii], it[ii]);
    }
    ASSERT_EQ(std::size_t(2), read_data.number_of_channels());

    // Next chunk
    std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data_tuple1 = chunk_manager.next();
    data::TimeFrequency<Cpu, uint8_t>& read_data1 = *(std::get<0>(data_tuple1));
    ASSERT_EQ(10241U, read_data1.number_of_spectra());
    rit = read_data1.begin();
    it = test_data.begin();
    for(std::size_t ii=0; ii<20478; ++ii)
    {
        // [2+ii] because we are offset by 2 samples
        ASSERT_EQ(rit[ii], it[2+ii]);
    }
    ASSERT_EQ(std::size_t(2), read_data1.number_of_channels());

    // Next chunk
    std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data_tuple2 = chunk_manager.next();
    data::TimeFrequency<Cpu, uint8_t>& read_data2 = *(std::get<0>(data_tuple2));
    ASSERT_EQ(10241U, read_data2.number_of_spectra());
    rit = read_data2.begin();
    it = test_data.begin();
    for(std::size_t ii=0; ii<20476; ++ii)
    {
        // [4+ii] because we are offset by 4 samples
        ASSERT_EQ(rit[ii], it[4+ii]);
    }
    ASSERT_EQ(std::size_t(2), read_data2.number_of_channels());


}

TEST_F(SigProcDadaStreamTest, test_dada_stream_corrupted_header)
{
    // This test verifies correct behaviour occurs when the
    // header information is not in the expected sigproc format
    test_utils::TestDadaDB test_db( 10, 409600, 10, 4096);
    test_db.create();

    //Write Data
    Config config;
    std::string write_header("This is not SIGPROC format header!!!");
    DadaWriteClient writer(test_db.key(),[&](std::ostream& out){out << write_header;});
    std::vector<unsigned char> input_data(1024);
    auto it = input_data.begin();
    writer.write(it,input_data.end());
    writer.new_sequence();
    config.number_of_samples(512);
    config.dada_key(test_db.key());
    SigProcDadaStream stream(config);
    panda::DataManager<SigProcDadaStream> chunk_manager(stream);
    ASSERT_ANY_THROW(chunk_manager.next());
}

TEST_F(SigProcDadaStreamTest, test_matched_size_dada_stream)
{
    // The chunk to fill and the dada stream sequence are of the same size
    //
    test_utils::TestDadaDB test_db( 10, 409600, 10, 4096);
    test_db.create();
    Config config;

    std::size_t chunk_time_samples=10240;
    auto test_data = detail::test_data(chunk_time_samples,2);
    detail::SigProcDadaWriter writer(test_db.key());
    writer << test_data;
    writer.new_sequence();
    auto test_data2 = detail::test_data(chunk_time_samples,2);
    writer << test_data2;
    writer.new_sequence();
    writer << test_data2;
    writer.new_sequence();
    //writer << test_data2;
    //writer.new_sequence();

    // read the dada buffer data as a stream
    config.number_of_samples(10240);
    config.dada_key(test_db.key());
    SigProcDadaStream stream(config);
    panda::DataManager<SigProcDadaStream> chunk_manager(stream);
    ASSERT_FALSE(stream.process());
    std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data_tuple = chunk_manager.next();
    data::TimeFrequency<Cpu, uint8_t>& data = *(std::get<0>(data_tuple));
    auto it = test_data.begin();
    auto rit = data.begin();
    for(std::size_t ii=0; ii<10240U; ++ii)
    {
        ASSERT_EQ(rit[ii],it[ii]);
    }
    ASSERT_EQ(chunk_time_samples, data.number_of_spectra());
    ASSERT_EQ(test_data.number_of_channels(), data.number_of_channels());

    // ensure we can still get a new tuple (i.e no lock up)
    std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data_tuple1 = chunk_manager.next();
    data::TimeFrequency<Cpu, uint8_t>& data1 = *(std::get<0>(data_tuple1));
    ASSERT_EQ(chunk_time_samples, data1.number_of_spectra());
    it = test_data2.begin();
    rit = data1.begin();
    for(std::size_t ii=0; ii<10240U; ++ii)
    {
        ASSERT_EQ(rit[ii],it[ii]);
    }

    // ensure we can still get a new tuple (i.e no lock up)
    std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data_tuple2 = chunk_manager.next();
    data::TimeFrequency<Cpu, uint8_t>& data2 = *(std::get<0>(data_tuple2));
    ASSERT_EQ(chunk_time_samples, data2.number_of_spectra());
    it = test_data2.begin();
    rit = data2.begin();
    for(std::size_t ii=0; ii<10240U; ++ii)
    {
        ASSERT_EQ(rit[ii],it[ii]);
    }

}


TEST_F(SigProcDadaStreamTest, test_multithreaded_dada_stream)
{
    // Test to make sure that the stream keeps runnning even for smaller dada buffer.
    // The test verifies that as long as there are two threads that are reading and
    // writing data blocks, the stream continues to run until the end of data stream.
    //

    test_utils::TestDadaDB test_db( 3, 20480, 10, 4096);
    test_db.create();

    Config config;

    //Generating data
    std::size_t chunk_time_samples=40960;
    auto test_data = detail::test_data(chunk_time_samples,2);
    detail::SigProcDadaWriter writer(test_db.key());

    // Reading DADA buffer as a stream

    config.number_of_samples(40000);
    config.dada_key(test_db.key());
    std::thread t1([&]()
            {
            writer << test_data;
            writer.new_sequence();
            writer << test_data;
            writer.new_sequence();

            });

    std::thread t2([&]()
            {
            SigProcDadaStream stream(config);
            panda::DataManager<SigProcDadaStream> chunk_manager(stream);
            ASSERT_FALSE(stream.process());
            std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data_tuple = chunk_manager.next();
            data::TimeFrequency<Cpu, uint8_t>& data = *(std::get<0>(data_tuple));
            ASSERT_EQ(40000U, data.number_of_spectra());
            ASSERT_EQ(std::size_t(2), data.number_of_channels());
            std::tuple<std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>>> data_tuple1 = chunk_manager.next();
            data::TimeFrequency<Cpu, uint8_t>& data1 = *(std::get<0>(data_tuple1));
            ASSERT_EQ(std::size_t(2), data1.number_of_channels());
            });

    // Running thread
    t1.join();
    t2.join();
}


} // namespace test
} // namespace psrdada
} // namespace cheetah
} // namespace ska
