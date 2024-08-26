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
#include "cheetah/producers/SpCclSpeadProducer.h"


namespace ska {
namespace cheetah {
namespace producers {

template<typename DerivedType, typename TimeFrequencyT>
struct SpCclSpeadProducerCRTP<DerivedType, TimeFrequencyT>::NullCallback
{
    inline
    void operator()(std::shared_ptr<SpCclType> const&) const {}
};

template<typename DerivedType, typename TimeFrequencyT>
std::shared_ptr<typename SpCclSpeadProducerCRTP<DerivedType, TimeFrequencyT>::SpCclType>
SpCclSpeadProducerCRTP<DerivedType, TimeFrequencyT>::DataFactory::operator()()
{
    return _producer.template get_chunk<SpCclType>();
}

template<typename DerivedType, typename TimeFrequencyT>
SpCclSpeadProducerCRTP<DerivedType, TimeFrequencyT>::SpCclSpeadProducerCRTP(io::exporters::SpCclSpeadReaderConfig const& config)
    : _data_factory(*this)
    , _reader(config, NullCallback(), _data_factory)
{
}

template<typename DerivedType, typename TimeFrequencyT>
SpCclSpeadProducerCRTP<DerivedType, TimeFrequencyT>::~SpCclSpeadProducerCRTP()
{
}

template<typename DerivedType, typename TimeFrequencyT>
void SpCclSpeadProducerCRTP<DerivedType, TimeFrequencyT>::init()
{
    _reader.start();
}

template<typename DerivedType, typename TimeFrequencyT>
bool SpCclSpeadProducerCRTP<DerivedType, TimeFrequencyT>::process()
{
    _reader.process();
    return false; // always producing data
}

template<typename TimeFrequencyT>
SpCclSpeadProducer<TimeFrequencyT>::SpCclSpeadProducer(io::exporters::SpCclSpeadReaderConfig const& config)
    : BaseT(config)
{
}

} // namespace producers
} // namespace cheetah
} // namespace ska
