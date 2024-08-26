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
#ifndef SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADSTREAMTRAITS_H
#define SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADSTREAMTRAITS_H

#include "SpeadLoggingAdapter.h"

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief common definitions for SpCcl data over spead transport protocol
 * @details
 */

class SpCclSpeadStreamTraits : private SpeadLoggingAdapter
{
    public:
        SpCclSpeadStreamTraits() {};
        ~SpCclSpeadStreamTraits() {};

    protected:
        static constexpr int tf_data_id                             = 0x1000;
        static constexpr int ft_data_id                             = 0x1001;
        static constexpr int data_channel_1_id                      = 0x1004;
        static constexpr int data_channel_width_id                  = 0x1005;
        static constexpr int data_sample_interval_id                = 0x1006;
        static constexpr int data_number_of_channels_id             = 0x1009; // SKA recommended value
        static constexpr int tf_data_start_time_id                  = 0x1600; // SKA recommended value
        static constexpr int ft_data_start_time_id                  = 0x1601;

        static constexpr int candidate_start_time_id                = 0x1020;
        static constexpr int candidate_dm_id                        = 0x1021;
        static constexpr int candidate_width_id                     = 0x1022;
        static constexpr int candidate_sigma_id                     = 0x1023;
        static constexpr int candidate_duration_id                  = 0x1024;

};


} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADSTREAMTRAITS_H
