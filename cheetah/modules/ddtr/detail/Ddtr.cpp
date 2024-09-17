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


template<typename ConfigType, typename NumericalRep>
Ddtr<ConfigType, NumericalRep>::Ddtr(BeamConfigType const& beam_config, ConfigType const& config, DedispersionHandler handler)
    : BaseT(beam_config, config, handler)
{
    //std::cout<<"Affinity: "<<beam_config.affinities().size()<<"\n";
    //if(beam_config.affinities().size())
    //{
    //    cpu_set_t cpuset;
    //    CPU_ZERO(&cpuset);
    //    CPU_SET(beam_config.affinities()[0], &cpuset);
    //    int rc = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    //    if (rc != 0) throw panda::Error("Thread: Error calling pthread_setaffinity_np: ");
    //}
}

template<typename ConfigType, typename NumericalRep>
Ddtr<ConfigType, NumericalRep>::~Ddtr()
{
    //_pool.wait();
}

template<typename ConfigType, typename NumericalRep>
template<typename TimeFreqDataT
       , typename data::EnableIfIsTimeFrequency<TimeFreqDataT, bool>>
void Ddtr<ConfigType, NumericalRep>::operator()(TimeFreqDataT const& tf_data)
{
    this->_buffer(tf_data);
}

template<typename ConfigType, typename NumericalRep>
template<typename T>
void Ddtr<ConfigType, NumericalRep>::operator()(std::shared_ptr<T> const& data)
{
    (*this)(*data);
}

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
