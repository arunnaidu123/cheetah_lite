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

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {


template<typename NumericalT>
SinglePulseImpl<NumericalT>::SinglePulseImpl(CheetahConfig<NumericalT> const& config, BeamConfigType<NumericalT> const& beam_config)
    : BaseT(config, beam_config)
    , _spdt_handler(*this)
    , _ddtr_handler(*this)
    , _spclusterer(config.sps_clustering_config())
    , _spsifter(config.spsift_config())
    , _spdt(beam_config, config.spdt_config(), [this](std::shared_ptr<SpType> sp_data)
                                            {
                                                _spdt_handler(sp_data);
                                            })
    , _ddtr(beam_config, config.ddtr_config(), [this](std::shared_ptr<DmTrialType> data)
                                            {
                                                _ddtr_handler(data);
                                            })
{
}

template<typename NumericalT>
SinglePulseImpl<NumericalT>::~SinglePulseImpl()
{
}

template<typename NumericalT>
SinglePulseImpl<NumericalT>::SpdtHandler::SpdtHandler(SinglePulseImpl<NumericalT>& p)
    : _pipeline(p)
{
}

template<typename NumericalT>
SinglePulseImpl<NumericalT>::DdtrHandler::DdtrHandler(SinglePulseImpl<NumericalT>& p)
    : _pipeline(p)
{
}

template<typename NumericalT>
void SinglePulseImpl<NumericalT>::operator()(TimeFrequencyType& data)
{
    _ddtr(data);
}

template<typename NumericalT>
void SinglePulseImpl<NumericalT>::SpdtHandler::operator()(std::shared_ptr<SpType> const& data) const
{
    _pipeline._thread.exec([this, data]()
                           {
                                _pipeline.do_post_processing(data);
                           }
                           );
}

template<typename NumericalT>
void SinglePulseImpl<NumericalT>::DdtrHandler::operator()(std::shared_ptr<DmTrialType> data)
{
    _pipeline._spdt(data);
}

template<typename NumericalT>
void SinglePulseImpl<NumericalT>::do_post_processing(std::shared_ptr<SpType> const& data)
{
    //PANDA_LOG<<"number of candidates "<<data->size();
    this->_spsifter(*data);
    std::shared_ptr<SpType> new_data = this->_spclusterer(data);
    this->out().send(ska::panda::ChannelId("sps_events"), new_data);
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
