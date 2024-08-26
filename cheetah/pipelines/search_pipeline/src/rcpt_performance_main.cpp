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
#include "cheetah/emulator/EmulatorAppConfig.h"
#include "cheetah/io/producers/rcpt/SkaPacketStreamFactory.h"
#include "cheetah/emulator/Emulator.h"
#include "cheetah/io/producers/rcpt/RcptTimerStats.h"
#include "cheetah/io/producers/rcpt/SkaUdpStream.h"
#include "cheetah/io/producers/rcpt/SkaUdpConfig.h"
#include "cheetah/io/producers/rcpt_mid/Config.h"
#include "cheetah/io/producers/rcpt_low/Config.h"
#include "panda/IpAddress.h"
#include "panda/TupleUtilities.h"
#include "panda/Log.h"
#include <memory>
#include <thread>
#include <chrono>
#include <type_traits>

class PerformanceTesterConfig : public ska::cheetah::emulator::EmulatorAppConfig
{
    public:
        template<typename StreamFactoryT>
        PerformanceTesterConfig(StreamFactoryT&& stream_factory)
            : ska::cheetah::emulator::EmulatorAppConfig("rcpt_performance_test"
                                                      , "execute performance tests for the UDP rcpt data ingest of cheetah "
                                                      , std::forward<StreamFactoryT>(stream_factory)
                                                       )
            , _rcpt_config(*this)
            , _iterations(10000)
        {
        }

        template<ska::cheetah::io::producers::rcpt::SkaSelector::Stream Stream>
        auto rcpt_config() -> typename ska::cheetah::io::producers::rcpt::SkaUdpStream<Stream>::Config&
        {
            return _rcpt_config.template rcpt_config<Stream>();
        }

        unsigned iterations() const {
            return _iterations;
        }

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override
        {
            add_options
                ("iter", boost::program_options::value<unsigned>(&_iterations)->default_value(_iterations), "number of chunks to process for each test");
            EmulatorAppConfig::add_options(add_options);
        }


    private:
        ska::cheetah::io::producers::rcpt::SkaUdpConfig _rcpt_config;
        unsigned _iterations;
};

typedef std::chrono::high_resolution_clock ClockType;

template<ska::cheetah::io::producers::rcpt::SkaSelector::Stream Stream>
struct SetEndPoint {
    static inline
    void exec(PerformanceTesterConfig& app_config, boost::asio::ip::udp::endpoint const& endpoint) {
        app_config.rcpt_config<Stream>().remote_end_point(endpoint);
    }
};

template<ska::cheetah::io::producers::rcpt::SkaSelector::Stream Stream>
struct RcptLaunch {
    template<class StreamFactoryT>
    static inline void exec(PerformanceTesterConfig& app_config, StreamFactoryT& stream_factory)
    {
        // setup a datamanager with the UdpStream we are interested in
        typedef typename ska::cheetah::io::producers::rcpt::SkaUdpStream<Stream>::UdpStream UdpStream;
        UdpStream stream(app_config.template rcpt_config<Stream>());
        ska::panda::DataManager<UdpStream> dm(stream);

        // now we have a prot number on which we listen we set the emulator
        // to send to this port
        auto endpoint = stream.local_end_point();
        app_config.emulator_config().fixed_address(ska::panda::IpAddress(endpoint));
        auto emulator = stream_factory.construct(app_config.stream_type(), app_config.emulator_config());

        std::thread th([&]() { emulator->run(); });

        // ---------- tests -----------------
        ska::cheetah::io::producers::rcpt::RcptTimerStats<ClockType> stats;
        unsigned iter_max = app_config.iterations();
        for(unsigned iter=0; iter < iter_max; ++iter)
        {
            auto data=dm.next();
            typedef typename ska::cheetah::io::producers::rcpt::SkaUdpStream<Stream>::DataType DataType;
            stats(*std::get<ska::panda::Index<std::shared_ptr<DataType>, typename decltype(dm)::DataSetType>::value>(data), ClockType::now());
        }
        PANDA_LOG << "\n----\n\t" << stats << "\n----";
        emulator->stop();
        th.join();
    }
};


int main(int argc, char** argv) {
    ska::cheetah::io::producers::rcpt::SkaPacketStreamFactory stream_factory;
    PerformanceTesterConfig app_config(stream_factory);
    int rv;
    try {
        // parse command line options
        if( (rv=app_config.parse(argc, argv)) ) return rv;

        // set the stream to listen on a random port
        ska::panda::IpAddress address(0, "127.0.0.1");
        boost::asio::ip::udp::endpoint local_endpoint = address.end_point<boost::asio::ip::udp::endpoint>();
        ska::cheetah::io::producers::rcpt::SkaSelector::select<void, SetEndPoint>(app_config.stream_type(), app_config, local_endpoint);
        ska::cheetah::io::producers::rcpt::SkaSelector::select<void, RcptLaunch>(app_config.stream_type(), app_config, stream_factory);
    }
    catch(std::exception const& e) {
        std::cerr << "Emulator: " << e.what() << std::endl;
    }
    catch(...) {
        std::cerr << "Emulator: unknown exception caught" << std::endl;
    }
    return 1;
}
