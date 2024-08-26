/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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
#include "cheetah/data/test_utils/FrequencySeriesTester.h"
#include "cheetah/data/FrequencySeries.h"
#include "cheetah/data/ComplexTypeTraits.h"
#include <panda/TypeTraits.h>
#include <panda/test/TestArch.h>
#include <random>
#include <numeric>

namespace ska {
namespace cheetah {
namespace data {
namespace test {

namespace {
    template<class Alloc, typename DeviceType, typename Enable>
    struct ConstructAllocatorHelper;

    template<class Alloc, typename DeviceType>
    struct ConstructAllocatorHelper<Alloc, DeviceType, std::true_type>
    {
          static inline
          Alloc construct(DeviceType& device)
          {
              return Alloc(device);
          }
    };

    template<class Alloc, typename DeviceType>
    struct ConstructAllocatorHelper<Alloc, DeviceType, std::false_type> {
          static inline
          Alloc construct(DeviceType&)
          {
              return Alloc();
          }
    };
    template<class Alloc, typename DeviceType>
    struct ConstructAllocator
    {
      private:
          template<typename T>
          using HasDeviceConstructorT = decltype(T(std::declval<DeviceType&>()));

      public:
          static inline
          Alloc construct(DeviceType& device)
          {
              return ConstructAllocatorHelper<Alloc, DeviceType, typename std::conditional<panda::HasMethod<Alloc,HasDeviceConstructorT>::value, std::true_type, std::false_type>::type>::construct(device);
          }
   };

    template<typename T, typename AllocT, typename AllocB_T>
    void verify_host_frequency_series_data(data::FrequencySeries<Cpu, T, AllocT> const& series_a, data::FrequencySeries<Cpu, T, AllocB_T> const& series_b)
    {
        ASSERT_EQ(series_a.frequency_step(), series_b.frequency_step());
        ASSERT_EQ(series_a.size(), series_b.size());
        auto copy_it=series_a.begin();
        auto it=series_b.begin();
        std::size_t count=0;
        while(it!=series_b.end()) {
            SCOPED_TRACE("element number: " + std::to_string(count));
            ASSERT_EQ(*it, *copy_it);
            ++it;
            ++copy_it;
            ++count;
        }
    }

    template <typename Arch, typename ValueType>
    void generate_and_transform(FrequencySeries<Arch, ValueType, std::allocator<ValueType>> &data)
    {
        std::default_random_engine generator;
        std::normal_distribution<float> distribution(0.0, 25.0);
        std::transform(data.begin(), data.end(), data.begin(), [&](ValueType const &)
                       { return static_cast<ValueType>(distribution(generator)); });
    }

    template <typename Arch, typename ValueType>
    void generate_and_transform(FrequencySeries<Arch, typename std::complex<ValueType>, std::allocator<typename std::complex<ValueType>>> &data)
    {
        std::default_random_engine generator;
        std::normal_distribution<float> real_distribution(0.0, 25.0);
        std::normal_distribution<float> imag_distribution(0.0, 25.0);

        std::transform(data.begin(), data.end(), data.begin(), [&](std::complex<ValueType> const &)
                       { return std::complex<ValueType>(static_cast<ValueType>(real_distribution(generator)),static_cast<ValueType>(imag_distribution(generator))); });
    }

} // namespace


template<typename FrequencySeriesT, typename OtherFrequencySeriesT>
typename FrequencySeriesTesterTraits<FrequencySeriesT, OtherFrequencySeriesT>::Allocator FrequencySeriesTesterTraits<FrequencySeriesT, OtherFrequencySeriesT>::allocator(panda::PoolResource<Arch>& device)
{
    return ConstructAllocator<Allocator, panda::PoolResource<Arch>>::construct(device);
}

template<typename FrequencySeriesTesterTraitsT>
FrequencySeriesTester<FrequencySeriesTesterTraitsT>::FrequencySeriesTester()
{
}

template<typename FrequencySeriesTesterTraitsT>
typename FrequencySeriesTester<FrequencySeriesTesterTraitsT>::TestSystem& FrequencySeriesTester<FrequencySeriesTesterTraitsT>::system()
{
    return _system;
}

TYPED_TEST_P(FrequencySeriesTester, construct_resize)
{
    typedef TypeParam Traits;
    typedef typename Traits::FrequencySeriesType FrequencySeriesType;

    Traits traits;
    for(auto& device : this->_system.devices()) {
        FrequencySeriesType series(traits.allocator(*device));
        ASSERT_EQ(series.size(), std::size_t(0));
        // increase size
        series.resize(2);
        ASSERT_EQ(series.size(), std::size_t(2));
        // reduce ize
        series.resize(1);
        ASSERT_EQ(series.size(), std::size_t(1));
        // resize to exisiting size should do nothing
        auto it = series.begin();
        series.resize(1);
        ASSERT_EQ(series.size(), std::size_t(1));
        ASSERT_EQ(it, series.begin());
    }
}

TYPED_TEST_P(FrequencySeriesTester, begin_end)
{
    typedef TypeParam Traits;
    typedef typename Traits::FrequencySeriesType FrequencySeriesType;

    Traits traits;
    std::size_t n=2;
    for(auto& device : this->_system.devices()) {
        FrequencySeriesType series(n, traits.allocator(*device));

        // begin end on non-const object
        std::size_t count=0;
        {
            auto it = series.begin();
            ASSERT_EQ(n, std::size_t(std::distance(it, series.end())));
            while(it != series.end()) {
                ++count;
                ++it;
            }
            ASSERT_EQ(count, n);
        }

        // begin end on const object
        const FrequencySeriesType& const_series = series;
        {
            count=0;
            auto it = const_series.begin();
            ASSERT_EQ(n, std::size_t(std::distance(it, const_series.end())));
            while(it != const_series.end()) {
                ++count;
                ++it;
            }
            ASSERT_EQ(count, n);
        }

        // cbegin cend on const object
        {
            count=0;
            auto it = const_series.cbegin();
            ASSERT_EQ(n, std::size_t(std::distance(it, series.cend())));
            while(it != const_series.cend()) {
                ++count;
                ++it;
            }
            ASSERT_EQ(count, n);
        }
    }
}

TYPED_TEST_P(FrequencySeriesTester, host_conversion)
{
    // test constructor that takes a host based FrequencySeries<Cpu, ...> object to ensure data is transfered
    // as expected
    typedef TypeParam Traits;
    typedef typename Traits::FrequencySeriesType FrequencySeriesType;
    typedef typename FrequencySeriesType::ValueType ValueType;
    typedef typename ConvertComplexNumber<cheetah::Cpu, ValueType>::type HostValueType;
    typedef FrequencySeries<panda::Cpu, HostValueType, std::allocator<HostValueType>> HostFrequencySeriesType;
    FourierFrequencyType df = 5.4 * hz;

    // some host based data
    std::size_t n=5;
    HostFrequencySeriesType host_data(df, n, std::allocator<ValueType>());
    generate_and_transform(host_data);

    Traits traits;
    for(auto& device : this->_system.devices()) {
        // copy host data to target FrequencySeries
        auto allocator = traits.allocator(*device);
        FrequencySeriesType series(host_data, allocator);
        ASSERT_EQ(series.size(), host_data.size());

        // copy back from target to host
        HostFrequencySeriesType host_data_copy(series);
        verify_host_frequency_series_data(host_data, host_data_copy);
    }
}

TYPED_TEST_P(FrequencySeriesTester, assignment_from_host)
{
    // tests the operator=(FrequencySeries const&) assignment operator
    // when used to copy from the host(Cpu) to the TypeParam defined Architecture
    typedef TypeParam Traits;
    typedef typename Traits::FrequencySeriesType FrequencySeriesType;
    typedef typename FrequencySeriesType::ValueType ValueType;
    typedef typename ConvertComplexNumber<cheetah::Cpu, ValueType>::type HostValueType;
    typedef FrequencySeries<panda::Cpu, HostValueType, std::allocator<HostValueType>> HostFrequencySeriesType;
    FourierFrequencyType df = 5.4 * hz;

    // some host based data
    std::size_t n=5;
    HostFrequencySeriesType host_data(df, n, std::allocator<ValueType>());
    generate_and_transform(host_data);

    Traits traits;
    for(auto& device : this->_system.devices()) {
        // copy host data to target FrequencySeries
        FrequencySeriesType series(traits.allocator(*device));
        series = host_data;

        ASSERT_EQ(series.size(), host_data.size());

        // copy back from target to host
        HostFrequencySeriesType host_data_copy;
        host_data_copy = series;

        verify_host_frequency_series_data(host_data, host_data_copy);
    }
}


TYPED_TEST_P(FrequencySeriesTester, copy_and_cast_test)
{
    // tests the copy and cast constructor
    // when used to copy from the host(Cpu) to the TypeParam defined Architecture
    typedef TypeParam Traits;
    typedef typename Traits::FrequencySeriesType FrequencySeriesType;
    typedef typename Traits::OtherFrequencySeriesType OtherFrequencySeriesType;
    typedef typename FrequencySeriesType::ValueType ValueType;
    typedef typename OtherFrequencySeriesType::ValueType OtherValueType;
    typedef typename FrequencySeriesType::Architecture Architecture;
    typedef typename OtherFrequencySeriesType::Architecture OtherArchitecture;
    typedef typename ConvertComplexNumber<cheetah::Cpu, OtherValueType>::type HostValueType;
    typedef FrequencySeries<panda::Cpu, HostValueType, std::allocator<HostValueType>> HostFrequencySeriesType;

    FourierFrequencyType df = 5.4 * hz;

    // some host based data
    std::size_t n=5;
    HostFrequencySeriesType host_data(df, n, std::allocator<OtherValueType>());

    generate_and_transform(host_data);

    Traits traits;
    for(auto& device : this->_system.devices()) {
        // copy host data to target FrequencySeries
        auto allocator = traits.allocator(*device);
        FrequencySeriesType series(host_data, allocator);

        // copy back from target to host
        HostFrequencySeriesType host_data_copy;
        host_data_copy = series;

        verify_host_frequency_series_data(host_data, host_data_copy);
    }
}


template<class TesterTraitsT, class ArchT>
struct InterDeviceAssignmentTester
{
    typedef typename TesterTraitsT::FrequencySeriesType FrequencySeriesType;
    typedef typename FrequencySeriesType::ValueType ValueType;
    typedef FrequencySeries<panda::Cpu, ValueType, std::allocator<ValueType>> HostFrequencySeriesType;
    typedef typename ConvertComplexNumber<ArchT, ValueType>::type ArchValueType;
    typedef FrequencySeries<ArchT, ArchValueType> ArchFrequencySeriesType;

    static inline
    void exec(FrequencySeriesTester<TesterTraitsT>& tester)
    {
        FourierFrequencyType df = 5.4 * hz;

        // create some data on the primary device
        std::size_t n=5;
        HostFrequencySeriesType host_data(df, n);
        std::iota(host_data.begin(), host_data.end(), 0); // fill with values 0, 1, 2,..

        panda::test::TestDevice<ArchT> system;

        for(auto& prim_device : tester.system().devices())
        {
            std::unique_ptr<FrequencySeriesType> src_series(create_data_type<FrequencySeriesType*>(*prim_device, host_data));

            // iterate over each device found of the target Arch
            for(auto& device : system.devices())
            {
                // copy host data to target FrequencySeries
                std::unique_ptr<ArchFrequencySeriesType> series(create_data_type<ArchFrequencySeriesType*>(*device));
                *series = *src_series;

                // verify data has been transfered
                HostFrequencySeriesType host_data_copy(*series);
                ASSERT_EQ(series->size(), host_data_copy.size());

                verify_host_frequency_series_data(host_data, host_data_copy);

                // copy back from target to primary
                *src_series = *series;
                ASSERT_EQ(series->size(), src_series->size());
            }
        }
    }
};


template<class TraitsT>
struct InterDeviceAssignmentTester<TraitsT, typename TraitsT::Arch>
{
    // dont execute any inter op tests for data with the same arch
    static inline void exec(FrequencySeriesTester<TraitsT>&) {}
};

TYPED_TEST_P(FrequencySeriesTester, assignment_inter_cuda)
{
#ifdef SKA_CHEETAH_ENABLE_CUDA
    InterDeviceAssignmentTester<TypeParam, cheetah::Cuda>::exec(*this);
#endif // SKA_CHEETAH_ENABLE_CUDA
}

REGISTER_TYPED_TEST_SUITE_P(FrequencySeriesTester, construct_resize, begin_end, host_conversion, copy_and_cast_test, assignment_from_host, assignment_inter_cuda);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
