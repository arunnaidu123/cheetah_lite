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
#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_SKAUDPCONFIG_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_SKAUDPCONFIG_H

#include "cheetah/io/producers/rcpt/SkaUdpStream.h"
#include <panda/ConfigModule.h>

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt {

/**
 * @brief A joint block for the various UDP streams available for the SKA telescope executables
 */

class SkaUdpConfig
{
    public:
        /**
         * @brief constructor adds the config nodes to the passed parent
         */
        SkaUdpConfig(panda::ConfigModule& parent);
        ~SkaUdpConfig() = default;

        /**
         * @brief return the rcpt configuration for the specified stream type
         * @tparam stream type for which the config applies
         * @details non-const version
         */
        template<rcpt::SkaSelector::Stream Stream>
        auto rcpt_config() -> typename rcpt::SkaUdpStream<Stream>::Config&;

        /**
         * @brief return the rcpt configuration for the specified stream type
         * @tparam stream type for which the config applies
         * @details const version
         */
        template<rcpt::SkaSelector::Stream Stream>
        auto rcpt_config() const -> typename rcpt::SkaUdpStream<Stream>::Config const&;

    private:
        template<rcpt::SkaSelector::Stream>
        struct RcptConfigHelper;

    private:
        rcpt_low::Config _rcpt_low_config;
        rcpt_mid::Config _rcpt_mid_config;
};


} // namespace rcpt
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska

#include "cheetah/io/producers/rcpt/detail/SkaUdpConfig.cpp"

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_SKAUDPCONFIG_H
