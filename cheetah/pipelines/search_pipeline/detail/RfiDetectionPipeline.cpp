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
#include "cheetah/pipelines/search_pipeline/RfiDetectionPipeline.h"
#include "cheetah/pipelines/search_pipeline/CheetahConfig.h"
#include <panda/TypeTraits.h>


namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {


template<typename NumericalT, typename RfimOutputHandler, typename RfiPolicy>
template<typename... HandlerArgs>
RfiDetectionPipeline<NumericalT, RfimOutputHandler, RfiPolicy>::RfiDetectionPipeline(CheetahConfig<NumericalT> const& config, BeamConfigType<NumericalT> const& beam_config, HandlerArgs&&... handler_args)
    : BaseT(config, beam_config)
    , _rfim_handler(*this, std::forward<HandlerArgs>(handler_args)...)
    , _bandpass_handler(*this)
    , _rfim(config.rfim_config(), _rfim_handler, _bandpass_handler)
    , _data_sequence(300)
    , _last_data_sequence_index(_data_sequence.size(), 0)
    , _next_data_sequence_index(_data_sequence.size(), 0)
{
}

template<typename NumericalT, typename RfimOutputHandler, typename RfiPolicy>
RfiDetectionPipeline<NumericalT, RfimOutputHandler, RfiPolicy>::~RfiDetectionPipeline()
{
    while(_last_data_sequence_index != _next_data_sequence_index) {
        std::this_thread::yield();
    }
}

template<typename NumericalT, typename RfimOutputHandler, typename RfiPolicy>
RfimOutputHandler& RfiDetectionPipeline<NumericalT, RfimOutputHandler, RfiPolicy>::output_handler()
{
    return _rfim_handler._output;
}

template<typename NumericalT, typename RfimOutputHandler, typename RfiPolicy>
void RfiDetectionPipeline<NumericalT, RfimOutputHandler, RfiPolicy>::operator()(TimeFrequencyType& data)
{
    // assumes we will only be called from a single thread
    _data_sequence[_last_data_sequence_index] = &data;
    ++_last_data_sequence_index;
    bool hold=false;
    // check we are not overruning the sequence buffer.
    // Check needs to happen before we launch job to avoid race cdn
    if(_last_data_sequence_index == _next_data_sequence_index) {
        hold = true;
    }
    try {
        _rfim.run(data);
    }
    catch(...)
    {
        --_last_data_sequence_index;
        throw;
    }

    if(hold) {
        // slow things down until buffer has caught up
        if(_last_data_sequence_index == _next_data_sequence_index) {
            PANDA_LOG_WARN << "sequencing queue full. Slowing pipeline until space recovered";
        }
        while(_last_data_sequence_index == _next_data_sequence_index) {
            std::this_thread::yield();
        }
    }
}

template<typename NumericalT, typename RfimOutputHandler, typename RfiPolicy>
template<typename... HandlerArgs>
RfiDetectionPipeline<NumericalT, RfimOutputHandler, RfiPolicy>::RfiOutputHandler::RfiOutputHandler(RfiDetectionPipeline& pipeline, HandlerArgs&&... args)
    : _pipeline(pipeline)
    , _output(std::forward<HandlerArgs>(args)...)
{
}

template<typename NumericalT, typename RfimOutputHandler, typename RfiPolicy>
void RfiDetectionPipeline<NumericalT, RfimOutputHandler, RfiPolicy>::RfiOutputHandler::operator()(ReturnType data)
{
    TimeFrequencyType& tf_data = static_cast<TimeFrequencyType&>(panda::is_pointer_wrapper<typename std::remove_reference<ReturnType>::type>::extract(data));

    // resync data stream
    while(_pipeline._data_sequence[_pipeline._next_data_sequence_index] != &tf_data) {
        std::this_thread::yield();
    }

    try {
        // process the rfim data
        _pipeline.out().send(panda::ChannelId("rfim"), tf_data);
        _output(data);
    }
    catch(...)
    {
        ++_pipeline._next_data_sequence_index;
        throw;
    }

    // now safe to let other threads push their data
    ++_pipeline._next_data_sequence_index;

}

template<typename NumericalT, typename RfimOutputHandler, typename RfiPolicy>
RfiDetectionPipeline<NumericalT, RfimOutputHandler, RfiPolicy>::BandPassOutputHandler::BandPassOutputHandler(RfiDetectionPipeline& pipeline)
    : _pipeline(pipeline)
{
}

template<typename NumericalT, typename RfimOutputHandler, typename RfiPolicy>
void RfiDetectionPipeline<NumericalT, RfimOutputHandler, RfiPolicy>::BandPassOutputHandler::operator()(typename BandPassOutputHandler::BandPassType const& data)
{
    static const panda::ChannelId bandpass_id("bandpass");
    _pipeline.out().send(bandpass_id, data);
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
