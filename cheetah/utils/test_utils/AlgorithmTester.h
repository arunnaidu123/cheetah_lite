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
#ifndef SKA_CHEETAH_TEST_UTILS_ALGORITHMTESTER_H
#define SKA_CHEETAH_TEST_UTILS_ALGORITHMTESTER_H

#include "panda/System.h"
#include "panda/arch/nvidia/Nvidia.h"
#include "panda/test/TestResourcePool.h"

#include <gtest/gtest.h>
#include <functional>

/**
 * @brief
 *    Macro for defining unit tests that require an accelerated device
 *    This provides a device named 'device' to your code
 * @example
 * ALGORITHM_TYPED_TEST_P(RfiTester, single_band_interference_spike)
 * {
 *      // you have access to the device via @param devuice
 *      // and also TypeParam is available. See Type Param tests in google docs for more info
 *      // n.b. the scope is not that of the fixture class Tester and so no access to members
 *      // or methods of that class
 * }
 */
#define ALGORITHM_TYPED_TEST_P(CaseName ,TestName) \
  template<typename TestTraits> void _algorithm_test_case_##CaseName##TestName(typename TestTraits::DeviceType&); \
  TYPED_TEST_P(CaseName , TestName) \
  { \
    ASSERT_NE(0U, this->devices().size()) << "no suitable devices"; \
    unsigned count = 0; \
    for( auto & device : this->devices()) { \
        std::cout << "running on device " << count++ << std::endl; \
        device->run([](typename std::decay<decltype(device)>::type::element_type & device) { _algorithm_test_case_##CaseName##TestName<TypeParam>(device); } ); \
    } \
  } \
  template<typename TypeParam> void _algorithm_test_case_##CaseName##TestName(typename TypeParam::DeviceType& device)

/**
 * @brief
 *    Macro for defining unit tests that require an accelerated device via a ResourcePool
 *    This provides a pool containing a single device via the argument 'pool' to your code
 * @example
 * POOL_ALGORITHM_TYPED_TEST_P(RfiTester, single_band_interference_spike)
 * {
 *      // you have access to the device via @param pool
 *      // and also TypeParam is available. See Type Param tests in google docs for more info
 *      // n.b. the scope is not that of the fixture class Tester and so no access to members
 *      // or methods of that class
 * }
 */
#define POOL_ALGORITHM_TYPED_TEST_P(CaseName ,TestName) \
  template<typename TestTraits> void _pool_algorithm_test_case_##CaseName##TestName(typename TestTraits::PoolType); \
  TYPED_TEST_P(CaseName , TestName) \
  { \
    ASSERT_NE(0U, this->devices().size()) << "no suitable devices"; \
    unsigned count = 0; \
    for( auto & device : this->devices()) { \
        typename TypeParam::PoolType pool; \
        pool.template add_resource<typename TypeParam::Arch>(device); \
        std::cout << "running on pool with device " << count++ << std::endl; \
        _pool_algorithm_test_case_##CaseName##TestName<TypeParam>(std::move(pool)); \
    } \
  } \
  template<typename TypeParam> void _pool_algorithm_test_case_##CaseName##TestName(typename TypeParam::PoolType pool)

namespace ska {
namespace cheetah {
namespace utils {
namespace test {

/**
 * @brief
 *    specified how to tell if a given device is capable enough.
 *    The ResourceCapability is specifed in the AlgorithmTesterTraits
 *    class for each algorith
 */
template <typename DeviceType, typename Traits>
struct supports {
    /// returns true if the provided device is compatible with the algorith to be tested
    static bool compatible(DeviceType const&);
};

/**
 * @brief describes the algorithm to be tested to the AlgorithmTester
 * @details The algorithTester is a template class that uses a passed type to
 *          apply the test using a specifc algorithm
 *          Inherit from the type for each device specific algorithm you want to test with.
 */
template <typename ArchitectureType,
          typename CapabilityType,
          typename DeviceTypeParam=panda::PoolResource<ArchitectureType>
          >
struct AlgorithmTesterTraits {
    /// The Architecture of the device to be used in the test
    typedef ArchitectureType Arch;
    /// The device type required to perform the test
    typedef DeviceTypeParam DeviceType;
    /// The device capability required for the test
    typedef CapabilityType Capability;
    /// The device type will be the main type passed a a type parameter
    typedef std::shared_ptr<panda::PoolResource<Arch>> TestParam;
};

template <typename ArchitectureType,
          typename CapabilityType=void,
          typename DeviceTypeParam=panda::PoolResource<ArchitectureType>,
          typename PoolTypeParam=panda::test::TestResourcePool<ArchitectureType>>
struct PoolAlgorithmTesterTraits {
    /// The Architecture of the device to be used in the test
    typedef ArchitectureType Arch;
    /// The device capability required for the test
    typedef CapabilityType Capability;
    /// The device type required to perform the test
    typedef DeviceTypeParam DeviceType;
    /// The type of resource pool contianing the test devices
    typedef PoolTypeParam PoolType;
    /// The pool type will be the main type passed a a type parameter
    typedef PoolType TestParam;
};

/**
 * @brief
 *    Base class for generic algortihm tests that require an accelerator device
 * @details
 *    THis class is responsible for finding and matching suitable devices to the algorithm requirements
 *    The actual tests should be writted with the ALGORITHM_TYPED_TEST_P macro above, or with the TYPED_TEST_P macro
 *    and running on the devices to test returned by the devices() method.
 */
template <typename Traits>
class AlgorithmTester : public ::testing::TestWithParam<typename Traits::TestParam>
{
    public:
        /**
         * @brief The Traits type for the Tester
         */
        typedef Traits AlgorithmTraits;

        /**
         * @brief The type of device required
         */
        typedef std::shared_ptr<panda::PoolResource<typename Traits::Arch>> DeviceType;

    public:
        AlgorithmTester();
        virtual ~AlgorithmTester();

    protected:
        /**
         * @brief all the devices matching DeviceType available for the test
         */
        std::vector<DeviceType>& devices() { return _devices; }

    private:
        std::vector<DeviceType> _devices;
};

} // namepsace test
} // namespace utils
} // namespace cheetah
} // namespace ska

#include "cheetah/utils/test_utils/detail/AlgorithmTester.cpp"

#endif // SKA_CHEETAH_TEST_UTILS_ALGORITHMTESTER_H
