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
#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_MID_CONFIG_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_MID_CONFIG_H

#include "cheetah/utils/Config.h"
#include "panda/EndpointConfig.h"
#include <panda/ProcessingEngine.h>
#include <panda/ProcessingEngineConfig.h>
#include <boost/asio/ip/udp.hpp>

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt_mid {

/**
 * @brief
 *    configurable parameters for the rcpt
 */

class Config : public utils::Config
{
    public:
        typedef panda::ProcessingEngine Engine;

    public:
        Config();
        ~Config();

        /**
         * @brief geters and setters for the UDP connection (IP address and port or remote peer)
         */
        boost::asio::ip::udp::endpoint remote_end_point() const;
        void remote_end_point(boost::asio::ip::udp::endpoint const& endpoint);

        Engine& engine() const;

        /**
         * @brief return the number of time samples in a chunk
         */
        unsigned spectra_per_chunk() const;

        /**
         * @brief set the number of time samples in a chunk
         */
        void spectra_per_chunk(unsigned samples);

        /**
         * @brief return the number of channels in a chunk
         */
        unsigned number_of_channels() const;

        /**
         * @brief set the number of channels in a chunk
         * @details NOTE that this has to correspond to the number actually being sent
         */
        void number_of_channels(unsigned samples);

        /**
         * @brief return the maximum number of UDP packet buffers to use
         * @details must be at least 1
         */
        std::size_t max_buffers() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        mutable std::unique_ptr<Engine> _engine;
        panda::ProcessingEngineConfig _engine_config;
        unsigned _number_of_channels;
        unsigned _spectra_per_chunk;
        std::size_t _max_buffer_count;
        ska::panda::EndpointConfig _endpoint_config; // listen address
};


} // namespace rcpt_mid
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_MID_CONFIG_H
