/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#include "cheetah/modules/sps/Sps.h"
#include "panda/ResourcePool.h"
#include "panda/TypeTraits.h"
#include "panda/Log.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace sps {


template<class ConfigType, typename NumericalRep>
Sps<ConfigType, NumericalRep>::Sps(ConfigType const& config, DedispersionHandler const& dm_handler, SpHandler const& sp_handler)
    : BaseT(config, dm_handler, sp_handler)
{
}


template<class ConfigType, typename NumericalRep>
Sps<ConfigType, NumericalRep>::~Sps()
{
    PANDA_LOG_DEBUG << "~Sps() destructor";
}

template<typename ConfigType, typename NumericalT>
template<typename TimeFreqDataT
       , typename data::EnableIfIsTimeFrequency<TimeFreqDataT, bool>>
void Sps<ConfigType, NumericalT>::operator()(TimeFreqDataT const& tf_data)
{
    static_cast<BaseT&>(*this)(tf_data);
}

template<class ConfigType, typename NumericalRep>
template<typename T
       , typename data::EnableIfIsTimeFrequency<T, bool>>
void Sps<ConfigType, NumericalRep>::operator()(std::shared_ptr<T> const& tf_data)
{
    static_cast<BaseT&>(*this)(*tf_data);
}

} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska
