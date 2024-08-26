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
#include "cheetah/io/producers/rcpt_mid/test/UdpStreamTimeFrequencyTest.h"
#include "cheetah/io/producers/rcpt_mid/UdpStreamTimeFrequency.h"
#include "cheetah/io/producers/rcpt_mid/BeamFormerDataTraits.h"
#include "cheetah/io/producers/rcpt_mid/Config.h"
#include "cheetah/io/producers/rcpt_mid/PacketGenerator.h"
#include "cheetah/io/producers/rcpt_mid/PacketGeneratorConfig.h"
#include "panda/DataManager.h"
#include "panda/IpAddress.h"
#include "panda/Log.h"
#include "panda/Engine.h"
#include <algorithm>
#include <deque>
#include <mutex>
#include <iostream>
#include <sstream>
#include <random>
#include <typeinfo>

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_mid {
namespace test {


UdpStreamTimeFrequencyTest::UdpStreamTimeFrequencyTest()
    : ::testing::Test()
{
}

UdpStreamTimeFrequencyTest::~UdpStreamTimeFrequencyTest()
{
}

void UdpStreamTimeFrequencyTest::SetUp()
{
}

void UdpStreamTimeFrequencyTest::TearDown()
{
}

struct TestModelTf {
    public:
        typedef data::FrequencyTime<Cpu, uint8_t> DataType;


        DataType& next(DataType& data) {
            //Generate random data
            std::default_random_engine generator;
            std::normal_distribution<float> distribution(128,50.0);
            // fill data with values
            std::unique_lock<std::mutex> lk(_mutex);
            std::generate(data.begin(), data.end(), [&]{ return ((typename DataType::DataType)distribution(generator)); } );
            _data.push_back(data);
            return data;
        }

        DataType sent_data() {
            std::lock_guard<std::mutex> lk(_mutex);
            if(_data.empty()) throw panda::Error("no more sent data");
            DataType d =  _data.front();
            _data.pop_front();
            return d;
        }

        bool has_sent_data() const {
            std::lock_guard<std::mutex> lk(_mutex);
            return !_data.empty();
        }

    private:
        mutable std::mutex _mutex;
        std::deque<DataType> _data;
};


template<typename Packet>
void test_udp_packets_stream_data_consistency_tf(std::size_t number_of_channels, std::size_t number_of_samples)
{
    typedef ska::panda::Connection<ska::panda::ConnectionTraits<ska::panda::Udp>> ConnectionType;
    typedef data::TimeFrequency<Cpu, int8_t> DataType;

    rcpt_mid::PacketGeneratorConfig generator_config;
    generator_config.interval(std::chrono::microseconds(10000));

    panda::IpAddress address(0, "127.0.0.1");
    boost::asio::ip::udp::endpoint local_endpoint = address.end_point<boost::asio::ip::udp::endpoint>();

    rcpt_mid::Config config;
    config.spectra_per_chunk(number_of_samples); // this is the number of time samples
    ASSERT_EQ(number_of_samples, config.spectra_per_chunk());


    SCOPED_TRACE(number_of_channels);
    SCOPED_TRACE("number_of_channels per time sample:");
    PANDA_LOG << "number_of_channles per time sample=" << number_of_channels;
    config.remote_end_point(local_endpoint);


    //Listening to the endpoint
    rcpt_mid::UdpStreamTimeFrequency stream(config);
    ska::panda::DataManager<rcpt_mid::UdpStreamTimeFrequency> dm(stream);
    auto endpoint = stream.local_end_point();

    // Start a UDP stream
    TestModelTf model;
    typedef PacketGenerator<TestModelTf> GeneratorType;
    GeneratorType generator(model, generator_config);
    panda::Engine& engine = config.engine();
    ConnectionType connection(engine); // to send packets
    connection.set_remote_end_point(endpoint);

    // test data throughput
    unsigned delta = 0U;
    unsigned received_count=0U;
    engine.poll(); // ensure the DataManager has completed all init jobs before we start testing throughput

    // send enough packets to fill a single chunk
    std::size_t packet_tot = delta;
    auto chunk_size = config.spectra_per_chunk()*config.number_of_channels();
    unsigned packet_count = 0U;
    // We send more packet than we really need as some OS's will drop the first packet if there is no ARP entry in the
    // cache (https://stackoverflow.com/questions/11812731/first-udp-message-to-a-specific-remote-ip-gets-lost) and will only
    // queue one more until resolved
    PANDA_LOG <<" chunk size "<<chunk_size;
    while(packet_tot < 2 * chunk_size )
    {
        ++packet_count;
        connection.send(generator.next(), [&](ska::panda::Error e){
                ASSERT_FALSE(e);
                ++received_count;
                });
        //std::this_thread::sleep_for(std::chrono::microseconds(100));
        engine.poll_one();
        packet_tot += 1152U;
    }

    delta = packet_tot - chunk_size;
    unsigned max_attempts=1000;
    while(received_count < packet_count) {
        engine.poll_one();
        if(--max_attempts == 0) FAIL() << "time out: expected packets not received";
    }
    auto data_out_ptr = std::get<0>(dm.next()); // keep it in context

    auto const& data_out = *data_out_ptr;
    data::FrequencyTime<Cpu, uint8_t> data_out_ft(data_out);
    auto it = data_out_ft.cbegin();
    //metadata tests
    boost::units::quantity<ska::cheetah::data::MegaHertz, double> bandwidth = static_cast<boost::units::quantity<ska::cheetah::data::MegaHertz, double>>(data_out.low_high_frequencies().second);
    ASSERT_EQ(bandwidth.value(),350.0);
    ASSERT_LT(data_out_ft.start_time(),ska::cheetah::utils::ModifiedJulianClock::now());
    ASSERT_GT(data_out_ft.start_time()+std::chrono::seconds(10),ska::cheetah::utils::ModifiedJulianClock::now());

    auto model_data = model.sent_data();
    auto sent_it = model_data.cbegin();
    auto sent_it_end = model_data.cend();

    ASSERT_NE(nullptr, &*data_out_ft.begin());
    ASSERT_FALSE(std::adjacent_find(data_out_ft.begin(), data_out_ft.end())== data_out_ft.end());
    while(sent_it!=sent_it_end)
    {
        ASSERT_EQ(true, *(it) == *sent_it || *(it) ==0 );
        ++it;
        ++sent_it;
    }
}

TEST_F(UdpStreamTimeFrequencyTest, test_udp_packets_stream_data_consistency_channels_size_equal_packet_size)
{
    typedef BeamFormerPacketMid Packet;
    test_udp_packets_stream_data_consistency_tf<Packet>(3700,8);
}



} // namespace test
} // namespace rcpt_mid
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
