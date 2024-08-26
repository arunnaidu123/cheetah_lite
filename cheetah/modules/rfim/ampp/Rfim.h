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
#ifndef SKA_CHEETAH_MODULES_RFIM_AMPP_RFIM_H
#define SKA_CHEETAH_MODULES_RFIM_AMPP_RFIM_H

#include "cheetah/modules/rfim/ampp/Config.h"
#include "cheetah/modules/rfim/ampp/Spectrum.h"
#include "cheetah/modules/rfim/PolicyInfo.h"
#include "cheetah/modules/rfim/policy/Policy.h"
#include "cheetah/utils/Architectures.h"
#include "cheetah/data/TimeFrequency.h"
#include <boost/circular_buffer.hpp>
#include <mutex>

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace ampp {

/**
 * @brief
 *   AMPP alogrithm CPU based on a dynamic bandfilter
 * @details
 *
 *   RFI interferance is characterised by spikes in the signal that are very narrow in band
 *   outside the normal levels of signal noise.
 *
 *   Consider a bandpass (assummed to be a continuous function):
 *   The bandpass is expected to be relatively stable from time sample to time sample,
 *   although will vary with climatic conditions over larger time scales.
 *
 *   |            ________
 *   |  ____------        -
 *   |                     ----
 *   |                         -
 *   |                          -------
 *   |
 *   +---------------------------------|
 *  fmin                              fmax
 *
 *  This represents the signal floor, (assuming the channel is functioning correctly) and we can
 *  expect the signal to actually be a noise modulated amplitude greater than this bandpass.
 *
 *  Any signal above the RMS of the noise must be treated with suspiscion.
 *
 *  Conceptually the algorithm breaks down the problem into the following stages:
 *
 *  stage 1: Estimation of the bandpass function
 *
 *  The incomming signal is scanned for the minimum values (excluding empty signals).
 *  These minumum values are taken as representatives of the bandpass function.
 *
 *  stage 2: Estimation of the noise mean & RMS
 *
 *  The rms across the whole range is considered to be constant
 *  The median across the whole channel range is considered to be a constant offset from the bandpass
 *
 *  stage 3:
 *  Signals greater than a cetain threshold (experessed as some multiple of the RMS) are deemed to be
 *  RMS and are excised (actual signal in that channel replaced with noise)
 *
 * --------- Operational Conditions ----------------------
 *
 *  F: Hardware Failure
 *  F2) Signal Empty across all channels
 *  F3) Signal Empty in a single channel
 *      @param channel
 *  F4) Signal Empty in a single block across more than one adjacent channel
 *     @param position of block
 *     @param number of channels
 *  F5) Signal Empty in multiple seperate blocks, each containing more than one channel
 *     @param number of blocks
 *     foreach block:
 *     @param position of block
 *     @param number of channels
 *
 *  P: External Sources Present
 *  P0) None - just noise
 *  P1) No Pulsars in signal
 *  P2) Regular Pulsar in signal
 *  P3) Multiple Pulsars in signal
 *  P4) Single Accelerated Pulsar in signal
 *  P5) Multiple Accelerated Pulsars in signal
 *  P6) Mixed regular pulsar and acelerated pulsar
 *
 *  R: RFI conditions
 *  R1) Single channel spike across multiple timesamples
 *      @param size of spike
 *  R2) Single channel spike in a single timesample
 *      @param size of spike
 *  R3) wide band spike across multiple channels in a single block
 *      @param size of spike
 *      @param size of block
 *      @param position of block
 *  R4) wide band spikes across multiple channels in multiple blocks of channels
 *      @param number of blocks
 *      foreach block:
 *      @param size of spike
 *      @param size of block
 *      @param position of block
 *
 *  Test Sceanarios:
 *  Test should be generated that combine each element from the sets F, P, and R, each with a
 *  representative array of parameters where appropriate. RFI should be detected without affecting
 *  the signal
 */

template<typename RfimTraits>
class Rfim
{
        typedef typename RfimTraits::NumericalRep NumericalRep;
        typedef typename RfimTraits::Policy Policy;
        typedef typename PolicyInfo<Policy>::AdapterType DataAdapter;
        static_assert(!std::is_const<DataAdapter>::value, "DataAdapter cannot be const");
        typedef typename PolicyInfo<Policy>::ReturnType ReturnType;

    public:
        typedef cheetah::Cpu Architecture;

    public:
        Rfim(Config const& config, typename RfimTraits::BandPassHandler const&);
        Rfim(Rfim const&) = delete;
        Rfim(Rfim&&);
        ~Rfim();

        /**
         * @brief set the maximum history
         */
        void set_max_history(unsigned length);

        /**
         * @brief task interface warpper around run
         */
        ReturnType operator()(panda::PoolResource<Cpu>& thread, std::shared_ptr<data::TimeFrequency<Cpu, NumericalRep>> data);

        /**
         * @brief perform rfi clipping on the data
         */
        ReturnType run(data::TimeFrequency<Cpu, NumericalRep>& data);

    private:
        void run_impl(data::TimeFrequency<Cpu, NumericalRep> const& data_read, DataAdapter& data_write);

    private:
        Config const& _config;
        unsigned _zero_dm;
        unsigned _bad_spectra;
        boost::circular_buffer<float> _mean_buffer, _rms_buffer;
        float _replacement_sample_mean, _replacement_sample_rms;
        float _rms_run_ave, _mean_run_ave;
        float _mean_over_rms;
        float _fraction_bad_channels;
        unsigned _sample_counter; // history pointer
        unsigned _max_history; // max size of history buffer
        // flag for removing median from each spectrum, equivalent to the zero-DMing technique
        int _use_mean_over_rms;
        std::shared_ptr<Spectrum<NumericalRep>> _bandpass_data_ptr;
        double _bandpass_rms;
        Spectrum<NumericalRep>& _bandpass_data;
        std::vector<NumericalRep> _replacement_sample; // TODO move into policy
        std::mutex _mutex;
        typename RfimTraits::BandPassHandler _bandpass_handler;
        policy::Policy<Policy> _policy;
};

} // namespace ampp
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "detail/Rfim.cpp"

#endif // SKA_CHEETAH_MODULES_RFIM_AMPP_RFIM_H
