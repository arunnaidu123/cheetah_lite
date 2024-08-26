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
#ifndef SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADSTREAMER_H
#define SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADSTREAMER_H

#ifdef ENABLE_SPEAD
#include "SpCclSpeadStreamerConfig.h"
#include "detail/SpCclSpeadStreamTraits.h"
#include "cheetah/data/SpCcl.h"
#include <panda/ChannelId.h>
#include <panda/ConnectionTraits.h>
#include <panda/Connection.h>
#include <spead2/common_defines.h>
#include <spead2/common_flavour.h>
#include <spead2/send_heap.h>
#include <spead2/send_udp.h>

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief
 * @details
 */

class SpCclSpeadStreamer : public SpCclSpeadStreamTraits
{
        typedef panda::ConnectionTraits<panda::Udp> ConnectionTraits;

    public:
        SpCclSpeadStreamer(SpCclSpeadStreamerConfig const&, panda::Engine&);
        SpCclSpeadStreamer(SpCclSpeadStreamer const&) = delete;
        ~SpCclSpeadStreamer();

        template<typename NumericalRep>
        SpCclSpeadStreamer& operator<<(data::SpCcl<NumericalRep> const&);

    private:
        SpCclSpeadStreamerConfig const& _config;
        spead2::flavour                 _sp_flavour;
        spead2::send::heap              _sp_end;
        spead2::send::udp_stream        _sp_stream;
        spead2::descriptor              _tf_data_desc;
        spead2::descriptor              _ft_data_desc;
        spead2::descriptor              _data_start_time_desc;
        spead2::descriptor              _data_number_of_channels_desc;
        spead2::descriptor              _data_sammple_interval;
        spead2::descriptor              _data_channel_1;
        spead2::descriptor              _data_channel_width;
        spead2::descriptor              _candidate_start_time_desc;
        spead2::descriptor              _candidate_dm_desc;
        spead2::descriptor              _candidate_width_desc;
        spead2::descriptor              _candidate_sigma_desc;
        spead2::descriptor              _candidate_duration_desc;
};

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska

#include "detail/SpCclSpeadStreamer.cpp"
#endif // ENABLE_SPEAD

#endif // SKA_CHEETAH_IO_EXPORTERS_SPCCLSPEADSTREAMER_H
