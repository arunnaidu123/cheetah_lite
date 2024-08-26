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
#ifndef SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADSTREAMERCONFIG_H
#define SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADSTREAMERCONFIG_H

#include "cheetah/utils/Config.h"
#include "panda/EndpointConfig.h"
#include "cheetah/data/CandidateWindowConfig.h"

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief
 * @details
 */

class SpCclSpeadStreamerConfig : public utils::Config
{
        typedef utils::Config BaseT;

    public:
        SpCclSpeadStreamerConfig();
        ~SpCclSpeadStreamerConfig();

        panda::IpAddress const& send_address() const;

        /**
         * @brief: the size of UDP packet to send
         */
        unsigned packet_size() const;

        /**
         * @brief: Getter for window config
         */
        data::CandidateWindowConfig const& window_config() const;

        /**
         * @brief: Setter for the candidate window config
         */
        void window_config(data::CandidateWindowConfig& window_config);

        /**
         * @brief Limit the data send rate in bytes per-second
         * @details You may need to do this where the kernel buffer sizes
         *          are limited to avoid dropped packets
         */
        float send_rate_limit() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        ska::panda::EndpointConfig _endpoint_config; // send address
        data::CandidateWindowConfig _window_config;
        unsigned _packet_size;
        float _send_rate;
};


} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADSTREAMERCONFIG_H
