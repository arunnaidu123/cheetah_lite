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
// For the GPU we use char instead of bool
typedef unsigned char LocalFlagsType;

#include "cheetah/modules/rfim/cuda/src/Kernels.cu"
#include "cheetah/modules/rfim/cuda/Rfim.h"
#include "panda/arch/nvidia/CudaDevicePointer.h"
#include "panda/Error.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace cuda {

template<typename TimeFrequencyDataType>
void flagger_freq_kernel_template(TimeFrequencyDataType const& data, unsigned char* host_flags)
{
    typedef typename std::decay<decltype(data)>::type::DataType DataType;

    std::size_t data_size=data.number_of_channels() * data.number_of_spectra();

    panda::nvidia::CudaDevicePointer<DataType> device_data(data_size);
    panda::nvidia::CudaDevicePointer<LocalFlagsType> device_flags(data_size);
    panda::nvidia::CudaDevicePointer<unsigned> device_nr_flagged(data.number_of_spectra());

    // Copy input data to device
    device_data.write(data.begin(), data.end());

    // Execute code on the device
    // Frequency Flagger
    flagger_freq<<<data.number_of_spectra(), data.number_of_channels(), (data.number_of_channels() * sizeof(DataType)) + (data.number_of_channels() * sizeof(LocalFlagsType))>>>(device_data(), device_flags(), device_nr_flagged(), data.number_of_channels(), data.number_of_spectra());

    // Copy flags data to host
    device_flags.read(host_flags);

}

void flagger_freq_kernel(data::TimeFrequency<Cpu, uint8_t> const& data, unsigned char* host_flags)
{
    flagger_freq_kernel_template(data, host_flags);
}

} // namespace cuda
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
