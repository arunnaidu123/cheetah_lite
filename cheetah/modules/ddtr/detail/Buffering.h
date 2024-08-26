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
#ifndef SKA_CHEETAH_MODULES_DDTR_BUFFERING_H
#define SKA_CHEETAH_MODULES_DDTR_BUFFERING_H

#include "CommonDedispersionPlan.h"
#include <memory>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

/**
 * @brief Buffering layer to collect sufficient data for the ddtr algorithm to run
 */

template<typename DdtrTraits, typename PlanType>
class Buffering
{
        typedef typename DdtrTraits::BufferFillerType BufferFillerType;
        typedef typename DdtrTraits::TimeFrequencyType TimeFrequencyType;
        typedef typename BufferFillerType::FullBufferHandlerT BufferFullCallBack;

    public:
        typedef PlanType Plan;

    public:
        template<typename... AggBufferArgs>
        Buffering(BufferFullCallBack const& callback
                , PlanType const& plan
                , std::size_t max_buffer_size
                , AggBufferArgs&&... agg_buffer_args
                );
        Buffering(Buffering const&) = delete;
        Buffering(Buffering&&) = delete;
        ~Buffering();

        template<typename DataT>
        void operator()(DataT const& data);

        void plan(std::unique_ptr<PlanType> plan);

        std::size_t remaining_capacity();

    private:
        void agg_buffer_init(TimeFrequencyType const&);

        template<typename DataT>
        void agg_buffer_fill(DataT&);

    private:
        data::DimensionSize<data::Time> _max_spectra;
        std::size_t _current_number_of_channels;
        BufferFillerType _agg_buf_filler;
        std::unique_ptr<PlanType> _plan;
};


} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "Buffering.cpp"

#endif // SKA_CHEETAH_MODULES_DDTR_BUFFERING_H
