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
#ifndef SKA_CHEETAH_EMULATOR_CONFIG_H
#define SKA_CHEETAH_EMULATOR_CONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/generators/Config.h"
#include "cheetah/io/producers/rcpt_low/PacketGeneratorConfig.h"
#include "cheetah/io/producers/rcpt_mid/PacketGeneratorConfig.h"
#include "panda/EndpointConfig.h"
#include "panda/Engine.h"
#include "panda/ServerConfig.h"
#include <boost/asio/ip/udp.hpp>
#include <string>
#include <vector>

namespace ska {
namespace cheetah {
namespace emulator {

/**
 * @brief
 *   Configuration options for the Emulator
 *
 * @details
 *
 */

class Config : public cheetah::utils::Config
{
    public:
        typedef boost::asio::ip::udp::endpoint EndPointType;

    public:
        Config();
        ~Config();

        /**
         * @brief the address to stream UDP packets to
         */
        EndPointType fixed_end_point() const;

        /**
         * @brief set the address to stream UDP packets to
         */
        void fixed_address(panda::IpAddress const&);

        /**
         * @brief the name of the data generator to use
         */
        std::string const& generator() const;


        io::producers::rcpt_low::PacketGeneratorConfig const& ska_low_config() const;

        io::producers::rcpt_mid::PacketGeneratorConfig const& ska_mid_config() const;

        /**
         * @brief the number of channels to generato
         */
        std::size_t number_of_channels() const;
        void number_of_channels(std::size_t n);

        /**
         * @brief the configuration block for generators
         */
        cheetah::generators::Config const& generators_config() const;

        panda::Engine& engine() const;
        panda::ServerConfig const& server_config() const;


    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        std::string _generator;
        std::size_t _number_of_channels;
        cheetah::generators::Config _generators_config;
        cheetah::io::producers::rcpt_low::PacketGeneratorConfig _ska_low_config;
        cheetah::io::producers::rcpt_mid::PacketGeneratorConfig _ska_mid_config;
        panda::ServerConfig _server_config;
        ska::panda::EndpointConfig _endpoint_config;

};

} // namespace emulator
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_EMULATOR_CONFIG_H
