/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 The SKA organisation
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


template<typename NumericalT, typename AccelerationSearchTraitsT,  template<typename> class RfimPolicyTemplate>
RfimAccelerationSearchImpl<NumericalT, AccelerationSearchTraitsT, RfimPolicyTemplate>::RfimAccelerationSearchImpl(CheetahConfig<NumericalT> const& config, BeamConfig<NumericalT> const& beam_config)
    : BaseT(config, beam_config)
    , _rfim(config, beam_config, *this)
{
}

template<typename NumericalT, typename AccelerationSearchTraitsT,  template<typename> class RfimPolicyTemplate>
RfimAccelerationSearchImpl<NumericalT, AccelerationSearchTraitsT, RfimPolicyTemplate>::~RfimAccelerationSearchImpl()
{
}

template<typename NumericalT, typename AccelerationSearchTraitsT,  template<typename> class RfimPolicyTemplate>
void RfimAccelerationSearchImpl<NumericalT, AccelerationSearchTraitsT, RfimPolicyTemplate>::operator()(TimeFrequencyType& data)
{
    _rfim(data); // apply RFI mitigation
}

template<typename NumericalT, typename AccelerationSearchTraitsT,  template<typename> class RfimPolicyTemplate>
RfimAccelerationSearchImpl<NumericalT, AccelerationSearchTraitsT, RfimPolicyTemplate>::RfimHandler::RfimHandler(RfimAccelerationSearchImpl<NumericalT, AccelerationSearchTraitsT, RfimPolicyTemplate>& rfim_pipeline)
    : _pipeline(rfim_pipeline)
{
}

template<typename NumericalT, typename AccelerationSearchTraitsT,  template<typename> class RfimPolicyTemplate>
void RfimAccelerationSearchImpl<NumericalT, AccelerationSearchTraitsT, RfimPolicyTemplate>::RfimHandler::operator()(RfimReturnType) const
{
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
