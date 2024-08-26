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
#include "cheetah/data/TimeSeries.h"
#include <panda/TypeTraits.h>


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
              return ConstructAllocatorHelper<Alloc, DeviceType
                                            , typename std::conditional<panda::HasMethod<Alloc,HasDeviceConstructorT>::value
                                                                      , std::true_type
                                                                      , std::false_type>::type>::construct(device);
          }
   };

    template<typename T, typename AllocT, typename AllocB_T>
    void verify_host_time_series_data(data::TimeSeries<Cpu, T, AllocT> const& series_a, data::TimeSeries<Cpu, T, AllocB_T> const& series_b)
    {
        ASSERT_EQ(series_a.sampling_interval(), series_b.sampling_interval());
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
}// namespace

template<typename TimeSeriesT>
typename TimeSeriesTesterTraits<TimeSeriesT>::Allocator TimeSeriesTesterTraits<TimeSeriesT>::allocator(panda::PoolResource<Arch>& device)
{
    return ConstructAllocator<Allocator, panda::PoolResource<Arch>>::construct(device);
}

template<typename TimeSeriesTesterTraitsT>
TimeSeriesTester<TimeSeriesTesterTraitsT>::TimeSeriesTester()
{
}

TYPED_TEST_P(TimeSeriesTester, construct_resize)
{
    // Test if we can resize a the TimeSeries object of the type specified by the Traits class
    typedef TypeParam Traits;
    typedef typename Traits::TimeSeriesType TimeSeriesType;

    Traits traits;
    for(auto& device : this->_system.devices()) {
        TimeSeriesType series(traits.allocator(*device));
        ASSERT_EQ(series.size(), std::size_t(0));

        // increase size
        series.resize(2);
        ASSERT_EQ(series.size(), std::size_t(2));

        // reduce size
        series.resize(1);
        ASSERT_EQ(series.size(), std::size_t(1));

        // resize to exisiting size should do nothing
        auto it = series.begin();
        series.resize(1);
        ASSERT_EQ(series.size(), std::size_t(1));
        ASSERT_EQ(it, series.begin());
    }
}

TYPED_TEST_P(TimeSeriesTester, begin_end)
{
    // We test the begin() and end() methods of the class and the Iterators which are returned by these methods
    // We test both const and non-const versions
    typedef TypeParam Traits;
    typedef typename Traits::TimeSeriesType TimeSeriesType;

    Traits traits;
    std::size_t n=2;
    for(auto& device : this->_system.devices()) {
        TimeSeriesType series(n, traits.allocator(*device));

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
        const TimeSeriesType& const_series = series;
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

TYPED_TEST_P(TimeSeriesTester, host_conversion)
{
    // ensure we can convert from a TimeSeries on the host to the TimeSeries type specified in the Traits
    // This tests the appropriate constructor
    typedef TypeParam Traits;
    typedef typename Traits::TimeSeriesType TimeSeriesType;
    typedef typename TimeSeriesType::ValueType ValueType;
    typedef TimeSeries<panda::Cpu, ValueType, std::allocator<ValueType>> HostTimeSeriesType;
    TimeType dt = 5.4 * seconds;

    // some host based data
    std::size_t n=5;
    HostTimeSeriesType host_data(dt, n, std::allocator<ValueType>());
    ValueType value=0;
    std::transform(host_data.begin(), host_data.end(), host_data.begin(), [&](ValueType const&) { return ++value; });

    Traits traits;
    for(auto& device : this->_system.devices()) {
        // copy host data to target TimeSeries
        auto allocator = traits.allocator(*device);
        TimeSeriesType series(host_data, allocator);
        ASSERT_EQ(series.size(), host_data.size());
        // copy back from target to host
        HostTimeSeriesType host_data_copy(series);
        verify_host_time_series_data(host_data, host_data_copy);
    }
}

TYPED_TEST_P(TimeSeriesTester, assignment_from_host)
{
    // tests the operator=(TimeSeries const&) assignment operator
    // when used to copy from the host(Cpu) to the TypeParam defined Architecture
    typedef TypeParam Traits;
    typedef typename Traits::TimeSeriesType TimeSeriesType;
    typedef typename TimeSeriesType::ValueType ValueType;
    typedef TimeSeries<panda::Cpu, ValueType, std::allocator<ValueType>> HostTimeSeriesType;
    TimeType dt = 5.4 * seconds;

    // some host based data
    std::size_t n = 5;
    HostTimeSeriesType host_data(dt, n, std::allocator<ValueType>());
    ValueType value = 0;
    std::transform(host_data.begin(), host_data.end(), host_data.begin(), [&](ValueType const&) { return ++value; });

    Traits traits;
    for(auto& device : this->_system.devices()) {
        // copy host data to target TimeSeries
        TimeSeriesType series(traits.allocator(*device));
        series = host_data;

        ASSERT_EQ(series.size(), host_data.size());

        // copy back from target to host
        HostTimeSeriesType host_data_copy;
        host_data_copy = series;

        ASSERT_EQ(host_data_copy.size(), n);
        verify_host_time_series_data(host_data, host_data_copy);
    }
}

REGISTER_TYPED_TEST_SUITE_P(TimeSeriesTester, construct_resize, begin_end, host_conversion, assignment_from_host);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
