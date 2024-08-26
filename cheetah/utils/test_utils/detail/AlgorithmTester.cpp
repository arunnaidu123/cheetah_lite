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
#include "cheetah/utils/test_utils/AlgorithmTester.h"
#include "cheetah/utils/System.h"
#include "cheetah/Configuration.h"
#include <panda/arch/nvidia/DeviceCapability.h>

namespace ska {
namespace cheetah {
namespace utils {
namespace test {

// generic helper to get devices of a specifc architecture
template <typename Arch>
struct get_devices {
    auto operator()() -> decltype( system().resources<Arch>()) {
        return system().resources<Arch>();
    }
};

// specialisation for Cpu - only want to run on a single thread
template <>
struct get_devices<ska::panda::Cpu> {
    auto operator()() -> std::decay<decltype(system().resources<ska::panda::Cpu>())>::type {
        auto devices = system().resources<ska::panda::Cpu>();
        if(devices.size() > 1) devices.resize(1);
        return devices;
    }
};

template <typename Traits>
AlgorithmTester<Traits>::AlgorithmTester()
{
    auto devices=get_devices<typename Traits::Arch>()();

    std::copy_if(devices.begin(),devices.end(),std::back_inserter(_devices),
		 [](std::shared_ptr<panda::PoolResource<typename Traits::Arch> > const& device){
		     return supports<typename Traits::DeviceType, Traits>::compatible(*device);
		 });

    if (!_devices.size()) throw panda::Error("No suitable device found to run test");
}

template <typename Traits>
AlgorithmTester<Traits>::~AlgorithmTester()
{
}

template<typename DeviceType, typename Traits> // device type and device capability are the same type
struct supports<panda::PoolResource<DeviceType>, Traits>
{
    static bool compatible(panda::PoolResource<DeviceType> const& )
    {
        return true;
    }
};

#ifdef SKA_CHEETAH_ENABLE_CUDA
/**
 * @brief Cuda GPU device is considered sufficient if the compute capabilities and the memory are sufficient
 */
template<typename Traits>
struct supports<panda::PoolResource<panda::nvidia::Cuda>, Traits>
{
    static bool compatible(panda::PoolResource<panda::nvidia::Cuda> const& device)
    {
        typedef typename Traits::Capability Capability;
        if(typename Capability::CapabilityVersion(device.device_properties().major, device.device_properties().minor) >=
                Capability::compute_capability()
            && (Capability::total_memory <= device.device_properties().totalGlobalMem))
            return true;
        return false;
    }
};
#endif // SKA_CHEETAH_ENABLE_CUDA

} // namespace test
} // namespace utils
} // namespace cheetah
} // namespace ska
