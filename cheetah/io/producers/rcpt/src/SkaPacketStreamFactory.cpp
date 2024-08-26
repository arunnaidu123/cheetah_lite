/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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
#include "cheetah/io/producers/rcpt/SkaPacketStreamFactory.h"
#include "cheetah/io/producers/rcpt_low/PacketGenerator.h"
#include "cheetah/io/producers/rcpt_mid/PacketGenerator.h"
#include "cheetah/emulator/Factory.h"
#include "cheetah/generators/DataGenerator.h"
#include "panda/Log.h"
#include <ostream>
#include <memory>


namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt {

template<typename PacketGeneratorType>
class EmulatorLauncher
{
        typedef typename PacketGeneratorType::SampleDataType NumericalRep;
        typedef ska::cheetah::emulator::Factory<PacketGeneratorType, NumericalRep> FactoryType;
        typedef typename PacketGeneratorType::DataType DataType;

    public:
        template<typename EmulatorConfig, typename StreamConfig>
        inline static
        std::unique_ptr<emulator::EmulatorBase>
        construct(EmulatorConfig const& config, StreamConfig&& stream_config) {
            ska::cheetah::generators::GeneratorFactory<DataType> generator_factory(config.generators_config());
            FactoryType factory(config, generator_factory);
            std::unique_ptr<emulator::EmulatorBase> emulator(factory.create(config.generator(), std::forward<StreamConfig>(stream_config)));
            PANDA_LOG << "emulator using generator: '" << config.generator() << "'";

            // start the emulator
            return emulator;
        }

        template<typename EmulatorConfig, typename StreamConfig>
        inline static
        int run(EmulatorConfig const& config, StreamConfig&& stream_config) {
            ska::cheetah::generators::GeneratorFactory<DataType> generator_factory(config.generators_config());
            FactoryType factory(config, generator_factory);
            std::unique_ptr<typename FactoryType::EmulatorType> emulator(factory.create(config.generator(), std::forward<StreamConfig>(stream_config)));
            PANDA_LOG << "emulator using generator: '" << config.generator() << "'";

            // start the emulator
            return emulator->run();
        }
};

template<>
struct SkaPacketStreamFactory::DataType<SkaPacketStreamFactory::Stream::SkaLow>
{
    typedef ska::cheetah::data::FrequencyTime<ska::cheetah::Cpu, rcpt_low::PssLowTraits::PacketDataType> type;
    typedef rcpt_low::PacketGenerator<ska::cheetah::generators::DataGenerator<type>> StreamType;
};

template<>
struct SkaPacketStreamFactory::DataType<SkaPacketStreamFactory::Stream::SkaMid>
{
    typedef ska::cheetah::data::FrequencyTime<ska::cheetah::Cpu, rcpt_mid::PssMidTraits::PacketDataType> type;
    typedef rcpt_mid::PacketGenerator<ska::cheetah::generators::DataGenerator<type>> StreamType;
};

template<>
struct SkaPacketStreamFactory::LaunchStream<SkaPacketStreamFactory::Stream::SkaLow> {
    static inline int exec(ska::cheetah::emulator::Config const& config)
    {
        typedef DataType<Stream::SkaLow>::StreamType StreamType;
        return EmulatorLauncher<StreamType>::run(config, config.ska_low_config());
    }
};

template<>
struct SkaPacketStreamFactory::LaunchStream<SkaPacketStreamFactory::Stream::SkaMid> {
    static inline int exec(ska::cheetah::emulator::Config const& config)
    {
        typedef DataType<Stream::SkaMid>::StreamType StreamType;
        return EmulatorLauncher<StreamType>::run(config, config.ska_mid_config());
    }
};

template<>
struct SkaPacketStreamFactory::ConstructStream<SkaPacketStreamFactory::Stream::SkaLow> {
    static inline std::unique_ptr<emulator::EmulatorBase> exec(ska::cheetah::emulator::Config const& config)
    {
        typedef DataType<Stream::SkaLow>::StreamType StreamType;
        return EmulatorLauncher<StreamType>::construct(config, config.ska_low_config());
    }
};

template<>
struct SkaPacketStreamFactory::ConstructStream<SkaPacketStreamFactory::Stream::SkaMid> {
    static inline std::unique_ptr<emulator::EmulatorBase> exec(ska::cheetah::emulator::Config const& config)
    {
        typedef DataType<Stream::SkaMid>::StreamType StreamType;
        return EmulatorLauncher<StreamType>::construct(config, config.ska_mid_config());
    }
};

template<SkaPacketStreamFactory::Stream StreamT>
bool SkaPacketStreamFactory::ListGenerators<StreamT>::exec(ska::cheetah::emulator::Config const& config)
{
    ska::cheetah::generators::GeneratorFactory<typename DataType<StreamT>::type> generator_factory(config.generators_config());
    for(auto const& item : generator_factory.available()) {
        std::cout << item << "\n";
    }
    return false;
}

bool SkaPacketStreamFactory::print_generators(std::string const& stream_name, ska::cheetah::emulator::Config const& config)
{
    return select<bool, ListGenerators>(stream_name, config);
}

int SkaPacketStreamFactory::exec(std::string const& stream_name, ska::cheetah::emulator::Config const& config)
{
    return select<int, LaunchStream>(stream_name, config);
}

std::unique_ptr<emulator::EmulatorBase>
SkaPacketStreamFactory::construct(std::string const& stream_name, ska::cheetah::emulator::Config const& config)
{
    return select<std::unique_ptr<emulator::EmulatorBase>, ConstructStream>(stream_name, config);
}

std::vector<std::string> SkaPacketStreamFactory::available()
{
    return SkaSelector::available();
}

} // namespace rcpt
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
