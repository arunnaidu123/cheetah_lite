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

#ifndef SKA_CHEETAH_DATA_CUDA_FREQUENCYTIME_H
#define SKA_CHEETAH_DATA_CUDA_FREQUENCYTIME_H

#include "cheetah/Configuration.h"

#ifdef SKA_CHEETAH_ENABLE_CUDA

#include "cheetah/data/cuda/TimeFrequency.h"
#include "cheetah/data/FrequencyTime.h"
#include "panda/arch/nvidia/DeviceMemory.h"

namespace ska {
namespace cheetah {
namespace data {

template <typename NumericalT>
class FrequencyTime<Cuda, NumericalT> : public TimeFrequencyCommon<FrequencyTime<Cuda, NumericalT>>
{
        typedef TimeFrequencyCommon<FrequencyTime<Cuda, NumericalT>> CommonBase;
        typedef panda::DeviceMemory<Cuda, NumericalT> ContainerType;

    public:
        /**
         * @brief the underlying data storage type for the amplitude of the signal
         */
        typedef NumericalT DataType;
        typedef NumericalT NumericalRep;

        typedef typename CommonBase::FrequencyType  FrequencyType;
        typedef typename CommonBase::TimeType  TimeType;
        typedef typename CommonBase::TimePointType TimePointType;

        typedef typename ContainerType::Iterator Iterator;
        typedef typename ContainerType::ConstIterator ConstIterator;

    public:
        FrequencyTime(FrequencyTime<Cuda, NumericalT> const& ft);

        template<typename OtherArch, typename OtherNumericalT>
        FrequencyTime(FrequencyTime<OtherArch, OtherNumericalT> const& ft);

        template <typename OtherNumericalT>
        FrequencyTime(TimeFrequency<Cuda, OtherNumericalT> const& tf);

        template <typename OtherNumericalT>
        FrequencyTime(TimeFrequency<Cpu, OtherNumericalT> const& tf);

        /**
         * @brief      Return an iterator pointing to the start of the FrequencyTime container
         */
        Iterator begin();
        ConstIterator begin() const;
        ConstIterator cbegin() const;

        /**
         * @brief      Return an iterator pointing to the end of the FrequencyTime container
         */
        Iterator end();
        ConstIterator end() const;
        ConstIterator cend() const;

        /**
        * @returns the integer number of channels in the block
        */
        std::size_t number_of_channels() const;

        /**
        * @returns the integer number of samples in the block
        */
        std::size_t number_of_spectra() const;

    private:
        ContainerType _data;
        std::size_t _number_of_channels;
        std::size_t _number_of_spectra;

};

}
}
}

#include "cheetah/data/cuda/detail/FrequencyTime.cpp"

#endif // SKA_CHEETAH_ENABLE_CUDA
#endif // SKA_CHEETAH_DATA_CUDA_FREQUENCYTIME_H
