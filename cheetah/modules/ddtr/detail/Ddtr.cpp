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
#include "cheetah/modules/ddtr/Ddtr.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {


template<typename BeamConfigType, typename ConfigType, typename NumericalRep>
Ddtr<BeamConfigType, ConfigType, NumericalRep>::Ddtr(BeamConfigType const& beam_config, ConfigType const& config, DedispersionHandler handler)
    : BaseT(beam_config, config, handler)
{
}

template<typename BeamConfigType, typename ConfigType, typename NumericalRep>
Ddtr<BeamConfigType, ConfigType, NumericalRep>::~Ddtr()
{
    //_pool.wait();
}

template<typename BeamConfigType, typename ConfigType, typename NumericalRep>
template<typename TimeFreqDataT
       , typename data::EnableIfIsTimeFrequency<TimeFreqDataT, bool>>
void Ddtr<BeamConfigType, ConfigType, NumericalRep>::operator()(TimeFreqDataT const& tf_data)
{
    this->_buffer(tf_data);
}

template<typename BeamConfigType, typename ConfigType, typename NumericalRep>
template<typename T>
void Ddtr<BeamConfigType, ConfigType, NumericalRep>::operator()(std::shared_ptr<T> const& data)
{
    (*this)(*data);
}

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
