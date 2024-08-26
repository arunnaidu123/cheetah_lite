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
#include "cheetah/modules/ddtr/detail/RfiExcisionFactory.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

template<typename TimeFrequencyType>
RfiExcisionFactory<TimeFrequencyType>::RfiExcisionFactory( RfiExcisionConfig const& config)
    :_config(config)
{
}

template<typename TimeFrequencyType>
RfiExcisionFactory<TimeFrequencyType>::~RfiExcisionFactory()
{
}

template<typename TimeFrequencyType>
typename RfiExcisionFactory<TimeFrequencyType>::Iterator RfiExcisionFactory<TimeFrequencyType>::begin( RfiFlagDataType& data)
{
    return Iterator(data, (std::size_t) 0U, _config.ideal_rms(), _config.threshold(), _config.active());
}

template<typename TimeFrequencyType>
typename RfiExcisionFactory<TimeFrequencyType>::ConstIterator RfiExcisionFactory<TimeFrequencyType>::begin( RfiFlagDataType const& data)
{
    return ConstIterator(data, (std::size_t) 0U, _config.ideal_rms(), _config.threshold(), _config.active());
}

template<typename TimeFrequencyType>
typename RfiExcisionFactory<TimeFrequencyType>::Iterator RfiExcisionFactory<TimeFrequencyType>::end(RfiFlagDataType& data)
{
    return Iterator(data, (data.tf_data().data_size()), _config.ideal_rms(), _config.threshold(), _config.active());
}

template<typename TimeFrequencyType>
typename RfiExcisionFactory<TimeFrequencyType>::ConstIterator RfiExcisionFactory<TimeFrequencyType>::end(RfiFlagDataType const& data)
{
    return ConstIterator(data, (data.tf_data().data_size()), _config.ideal_rms(), _config.threshold(), _config.active());
}

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
