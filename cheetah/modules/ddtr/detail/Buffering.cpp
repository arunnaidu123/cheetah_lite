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
#include "cheetah/modules/ddtr/detail/Buffering.h"
#include "panda/ResourcePool.h"
#include "panda/TypeTraits.h"
#include "panda/Log.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {


template<typename DdtrTraits, typename PlanType>
template<typename... AggBufferArgs>
Buffering<DdtrTraits, PlanType>::Buffering(BufferFullCallBack const& callback
                                , PlanType const& plan
                                , std::size_t max_buffer_size
                                , AggBufferArgs&&... agg_buffer_args)
    : _max_spectra(max_buffer_size)
    , _current_number_of_channels(0)
    , _agg_buf_filler(callback, std::forward<AggBufferArgs>(agg_buffer_args)...)
    , _plan(new PlanType(plan))
{
}

template<typename DdtrTraits, typename PlanType>
Buffering<DdtrTraits, PlanType>::~Buffering()
{
}

template<typename DdtrTraits, typename PlanType>
void Buffering<DdtrTraits, PlanType>::agg_buffer_init(TimeFrequencyType const& data)
{
    if (data.number_of_channels() != _current_number_of_channels)
    {

        data::DimensionSize<data::Time> dedisp_samples = _plan->dedispersion_strategy(data);
        data::DimensionSize<data::Time> number_of_spectra = dedisp_samples;
        if(dedisp_samples > _max_spectra && _max_spectra != 0) {
            PANDA_LOG_WARN << "algorithm algo plans ignored mac number of samples";
            //number_of_spectra = _plan->reset(_max_spectra); // TODO
        }

        data::DimensionSize<data::Time> overlap = _plan->buffer_overlap(); // in numbers of spectra
        if(overlap == 0) {
            PANDA_LOG_WARN << "buffer overlap is zero";
        }
        data::DimensionSize<data::Time> min_number_of_spectra(overlap + 1);

        if(min_number_of_spectra >= _max_spectra && _max_spectra != 0) {
            panda::Error e("configuration error: dedispersion plan requires at least ");
            e << min_number_of_spectra << " spectra. " << "Maximum possible (given algo and pool selection)=" << dedisp_samples;
            throw e;
        }

        if(overlap >= number_of_spectra) {
            PANDA_LOG_ERROR << "configuration error: samples requested < minimum required for dedispersion plan(" << overlap << ")";
            throw panda::Error("dedispersion sample buffer size is too small");
        }
        PANDA_LOG << "setting dedispersion buffer size to " << number_of_spectra << " spectra";
        //_agg_buf_filler.resize(data.number_of_channels() * number_of_spectra);
        _agg_buf_filler.resize(number_of_spectra, data::DimensionSize<data::Frequency>(data.number_of_channels()));
        _agg_buf_filler.metadata(data.metadata());

        PANDA_LOG << "setting buffer overlap to " << overlap << " spectra";
        //_agg_buf_filler.set_overlap(data.number_of_channels() * overlap);
        _agg_buf_filler.set_overlap(overlap);

        _current_number_of_channels = data.number_of_channels();
    }
}

template<typename DdtrTraits, typename PlanType>
template<typename DataT>
void Buffering<DdtrTraits, PlanType>::agg_buffer_fill(DataT& data)
{
    TimeFrequencyType const& tf_data = static_cast<TimeFrequencyType const&>(panda::is_pointer_wrapper<typename std::remove_reference<DataT>::type>::extract(data));
    agg_buffer_init(tf_data);
    _agg_buf_filler << data;
}

template<typename DdtrTraits, typename PlanType>
template<typename DataT>
void Buffering<DdtrTraits, PlanType>::operator()(DataT const& data)
{
    this->template agg_buffer_fill(data);
}

template<typename DdtrTraits, typename PlanType>
void Buffering<DdtrTraits, PlanType>::plan(std::unique_ptr<PlanType> plan)
{
    _plan = std::move(plan);
}

template<typename DdtrTraits, typename PlanType>
std::size_t Buffering<DdtrTraits, PlanType>::remaining_capacity()
{
    return _agg_buf_filler.remaining_capacity();
}

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
