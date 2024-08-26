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
#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_SKAPACKETSTREAMFACTORY_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_SKAPACKETSTREAMFACTORY_H

#include "cheetah/io/producers/rcpt/SkaSelector.h"
#include "cheetah/io/producers/rcpt_low/PacketGenerator.h"
#include "cheetah/io/producers/rcpt_mid/PacketGenerator.h"
#include "cheetah/emulator/Config.h"
#include "cheetah/data/FrequencyTime.h"
#include <memory>

namespace ska {
namespace cheetah {
namespace emulator {
    class EmulatorBase;
} // namespace emulator
namespace io {
namespace producers {
namespace rcpt {

/**
 * @brief facilities to generate and use the Ska specific UDP packet streams
 * @details
 */

class SkaPacketStreamFactory : protected SkaSelector
{
    public:
        /**
         * @brief print to std::cout the names of generators that can be used for the
         *        specified stream
         */
        static
        bool print_generators(std::string const& stream_name, ska::cheetah::emulator::Config const&);

        /**
         * @brief construct and launch the requested stream
         */
        static
        int exec(std::string const& stream_name, ska::cheetah::emulator::Config const& config);

        /**
         * @brief construct and launch the requested stream
         */
        static
        std::unique_ptr<emulator::EmulatorBase>
        construct(std::string const& stream_name, ska::cheetah::emulator::Config const& config);

        /**
         * @brief return the names of the available stream identifiers
         */
        static
        std::vector<std::string> available();

    private:
        template<Stream>
        struct DataType;

        template<Stream>
        struct LaunchStream;

        template<Stream>
        struct ConstructStream;

        template<Stream StreamT>
        struct ListGenerators {
            static inline bool exec(ska::cheetah::emulator::Config const& config);
        };

};


} // namespace rcpt
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
#include "cheetah/io/producers/rcpt/detail/SkaPacketStreamFactory.cpp"

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_SKAPACKETSTREAMFACTORY_H
