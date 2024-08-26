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
#include "cheetah/data/test_utils/SeriesTester.h"
#include "cheetah/data/Series.h"


namespace ska {
namespace cheetah {
namespace data {
namespace test {

template<typename T, typename AllocT, typename AllocB_T>
void verify_host_series_data(data::Series<Cpu, T, AllocT> const& series_a, data::Series<Cpu, T, AllocB_T> const& series_b)
{
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

template<typename SeriesTesterTraitsT>
SeriesTester<SeriesTesterTraitsT>::SeriesTester()
{
}

TYPED_TEST_P(SeriesTester, construct_resize)
{
    typedef TypeParam Traits;
    typedef typename Traits::SeriesType SeriesType;

    Traits traits;
    for(auto& device : this->_system.devices()) {
        SeriesType series(traits.allocator(*device));
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

TYPED_TEST_P(SeriesTester, begin_end)
{
    // test the begin() and end() operators on const nd non-const objects
    // and that the iterators returned perform the basic operations required of an iterator
    typedef TypeParam Traits;
    typedef typename Traits::SeriesType SeriesType;

    Traits traits;
    std::size_t n=2;
    for(auto& device : this->_system.devices()) {
        SeriesType series(n, traits.allocator(*device));

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
        const SeriesType& const_series = series;
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

TYPED_TEST_P(SeriesTester, host_conversion)
{
    // test constructor that takes a host based Series<Cpu, ...> object to ensure data is transfered
    // as expected
    typedef TypeParam Traits;
    typedef typename Traits::SeriesType SeriesType;
    typedef typename SeriesType::ValueType ValueType;
    typedef Series<panda::Cpu, ValueType, std::allocator<ValueType>> HostSeriesType;

    // some host based data
    std::size_t n=5;
    HostSeriesType host_data(n, std::allocator<ValueType>());
    ValueType value=0;
    std::transform(host_data.begin(), host_data.end(), host_data.begin(), [&](ValueType const&) { return ++value; });

    Traits traits;
    for(auto& device : this->_system.devices()) {
        // copy host data to target Series
        auto allocator = traits.allocator(*device);
        SeriesType series(host_data, allocator);
        ASSERT_EQ(series.size(), host_data.size());
        // copy back from target to host
        HostSeriesType host_data_copy(series);
        verify_host_series_data(host_data, host_data_copy);
    }
}

TYPED_TEST_P(SeriesTester, assignment_from_host)
{
    // tests the operator=(Series const&) assignment operator
    // when used to copy from the host(Cpu) to the TypeParam defined Architecture
    typedef TypeParam Traits;
    typedef typename Traits::SeriesType SeriesType;
    typedef typename SeriesType::ValueType ValueType;
    typedef Series<panda::Cpu, ValueType, std::allocator<ValueType>> HostSeriesType;

    // some host based data
    std::size_t n=5;
    HostSeriesType host_data(n, std::allocator<ValueType>());
    ValueType value=0;
    std::transform(host_data.begin(), host_data.end(), host_data.begin(), [&](ValueType const&) { return ++value; });

    Traits traits;
    for(auto& device : this->_system.devices()) {
        // copy host data to target Series using assignment allocator
        SeriesType series(traits.allocator(*device));
        series = host_data;

        ASSERT_EQ(series.size(), host_data.size());
        // copy back from target to host
        HostSeriesType host_data_copy(series);
        verify_host_series_data(host_data, host_data_copy);
    }
}

TYPED_TEST_P(SeriesTester, assignment_to_host)
{
    // tests the operator=(Series const&) assignment operator
    // when used to copy from the TypeParam defined Architecture to the host (Cpu)
    typedef TypeParam Traits;
    typedef typename Traits::SeriesType SeriesType;
    typedef typename SeriesType::ValueType ValueType;
    typedef Series<panda::Cpu, ValueType, std::allocator<ValueType>> HostSeriesType;

    // some host based data
    std::size_t n=5;
    HostSeriesType host_data(n, std::allocator<ValueType>());
    ValueType value=0;
    std::transform(host_data.begin(), host_data.end(), host_data.begin()
                , [&](ValueType const&)
                  {
                      return ++value;
                  });

    Traits traits;
    for(auto& device : this->_system.devices()) {
        // copy host data to target Series using copy assignment (assuming it work OK)
        auto allocator = traits.allocator(*device);
        SeriesType series(host_data, allocator);

        // copy back from target to host using assignment operator
        HostSeriesType host_data_copy;
        host_data_copy = series;

        verify_host_series_data(host_data, host_data_copy);
    }
}

REGISTER_TYPED_TEST_SUITE_P(SeriesTester, construct_resize, begin_end, host_conversion, assignment_from_host, assignment_to_host);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
