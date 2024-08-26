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
#ifndef SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADREADERCONFIG_H
#define SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADREADERCONFIG_H
#ifdef ENABLE_SPEAD
#include "cheetah/utils/Config.h"
#include "panda/EndpointConfig.h"
#include <panda/ProcessingEngine.h>
#include <panda/ProcessingEngineConfig.h>
#include <boost/asio/ip/udp.hpp>
#include <memory>


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief Configuration Options for the SpCclSpeadReader
 */

class SpCclSpeadReaderConfig : public utils::Config
{
        typedef utils::Config BaseT;
        typedef panda::ProcessingEngine Engine;
        typedef boost::asio::ip::udp::endpoint EndPointType;

    public:
        SpCclSpeadReaderConfig();
        ~SpCclSpeadReaderConfig();

        panda::IpAddress const& listen_address() const;
        Engine& engine() const;

        void endpoint(EndPointType const& end_point);
        EndPointType const& endpoint() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        ska::panda::EndpointConfig _endpoint_config; // send address
        mutable EndPointType _endpoint;
        panda::ProcessingEngineConfig _engine_config;
        bool _init;
        mutable std::unique_ptr<Engine> _engine;
};


} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
#endif // ENABLE_SPEAD

#endif // SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADREADERCONFIG_H
