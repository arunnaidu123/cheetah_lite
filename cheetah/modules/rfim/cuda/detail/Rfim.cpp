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
#include "cheetah/modules/rfim/cuda/Rfim.h"
#include "cheetah/Configuration.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace cuda {


template<typename RfimTraits>
Rfim<RfimTraits>::Rfim(Config const& )
{
}

template<typename RfimTraits>
Rfim<RfimTraits>::~Rfim()
{
}

#ifndef SKA_CHEETAH_ENABLE_CUDA

// dummy version
template<typename RfimTraits>
inline void Rfim<RfimTraits>::operator()(panda::PoolResource<Cuda>&, DataType&, DataAdapter&)
{
}
#else // SKA_CHEETAH_ENABLE_CUDA
void flagger_freq_kernel(data::TimeFrequency<Cpu, uint8_t> const&, unsigned char*);

namespace {
template<typename DataType>
struct CudaRfimExecutor
{
    template<typename DataAdapter, typename DataT>
    inline static void exec(panda::PoolResource<Cuda>&, DataT const&, DataAdapter&) {
        PANDA_LOG_WARN << "Rfim::cuda:: No algorithm available for this data type";
    }
};

template<>
struct CudaRfimExecutor<uint8_t>
{
    template<typename DataAdapter, typename DataT>
    inline static void exec(panda::PoolResource<Cuda>&, DataT const& data, DataAdapter&)
    {
        typedef typename std::decay<decltype(data)>::type::DataType DataType;
        typedef unsigned char LocalFlagsType;

        std::size_t data_size=data.number_of_channels() * data.number_of_spectra();
        if(data_size == 0) return;

        // Allocate host memory
        LocalFlagsType* host_flags = new LocalFlagsType[data_size];

        try {
            flagger_freq_kernel(data, host_flags);
            // Populate the TimeFrequencyFlags data structure
    /*
            for ( unsigned int channel = 0; channel < data.number_of_channels(); channel++ ) {
                auto channel_iterator = (data.get_flags())->channel(channel);
                unsigned int sample_number = 0;

                for ( auto sample = channel_iterator.begin(); sample != channel_iterator.end(); ++sample ) {
                    *sample = static_cast<data::FlagsType>(host_flags[(channel * data.number_of_spectra()) + sample_number]);
                    sample_number++;
                }
            }
    */
        } catch(...)
        {
            delete[] host_flags;
            throw;
        }
        delete[] host_flags;
        }
};
} // namespace

template<typename RfimTraits>
inline void Rfim<RfimTraits>::operator()(panda::PoolResource<Cuda>& device, DataType& data, DataAdapter& adapter)
{
    typedef typename std::decay<decltype(data)>::type::DataType NumericalT;
    CudaRfimExecutor<NumericalT>::exec(device, data, adapter);
}
#endif //SKA_CHEETAH_ENABLE_CUDA

} // namespace cuda
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
