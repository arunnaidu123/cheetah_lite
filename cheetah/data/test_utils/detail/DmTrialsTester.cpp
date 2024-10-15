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
#include "cheetah/data/test_utils/DmTrialsTester.h"
#include "cheetah/data/DmTrials.h"
#include <cmath>
#include <numeric>


namespace ska {
namespace cheetah {
namespace data {
namespace test {


template<typename DmTrialsTesterTraitsT>
DmTrialsTester<DmTrialsTesterTraitsT>::DmTrialsTester()
{
}

TYPED_TEST_P(DmTrialsTester, construct_nullptr_metadata)
{
    /*
    typedef TypeParam Traits;
    typedef typename Traits::DmTrialsType DmTrialsType;
    typedef typename DmTrialsType::TimeType TimeType;
    typedef typename DmTrialsType::DmType Dm;

    Traits traits;
    auto now = cheetah::utils::ModifiedJulianClock::now();
    std::shared_ptr<data::DmTrialsMetadata> metadata;

    for(auto& device : this->_system.devices()) {
        ASSERT_THROW(DmTrialsType dm_trials(metadata, now, traits.allocator(*device)), panda::Error);
    }
    */
}

TYPED_TEST_P(DmTrialsTester, construct_empty_metadata)
{
    /*
    typedef TypeParam Traits;
    typedef typename Traits::DmTrialsType DmTrialsType;
    typedef typename DmTrialsType::TimeType TimeType;
    typedef typename DmTrialsType::DmType Dm;

    Traits traits;
    auto now = cheetah::utils::ModifiedJulianClock::now();
    auto metadata = data::DmTrialsMetadata::make_shared(TimeType(0.000064*data::seconds),1<<6);

    for(auto& device : this->_system.devices()) {
        ASSERT_THROW(DmTrialsType dm_trials(metadata, now, traits.allocator(*device)), panda::Error);
    }
    */
}

TYPED_TEST_P(DmTrialsTester, begin_end)
{
    /*
    typedef TypeParam Traits;
    typedef typename Traits::DmTrialsType DmTrialsType;
    typedef typename DmTrialsType::DmType Dm;

    Traits traits;
    auto now = cheetah::utils::ModifiedJulianClock::now();
    std::size_t size=1<<6;
    auto metadata = data::DmTrialsMetadata::make_shared(TimeType(0.000064*data::seconds), size);
    metadata->emplace_back(Dm(0.0*data::parsecs_per_cube_cm),1);
    metadata->emplace_back(Dm(10.0*data::parsecs_per_cube_cm),2);
    metadata->emplace_back(Dm(20.0*data::parsecs_per_cube_cm),4);

    std::size_t n=metadata->size();
    for(auto& device : this->_system.devices()) {
        DmTrialsType dm_trials(metadata, now, traits.allocator(*device));

        // begin end on non-const object
        std::size_t count=0;
        {
            auto it = dm_trials.begin();
            ASSERT_EQ(n, std::size_t(std::distance(it, dm_trials.end())));
            while(it != dm_trials.end()) {
                ++count;
                ++it;
            }
            ASSERT_EQ(count, n);
        }

        // begin end on const object
        const DmTrialsType& const_dm_trials = dm_trials;
        {
            count=0;
            auto it = const_dm_trials.begin();
            ASSERT_EQ(n, std::size_t(std::distance(it, const_dm_trials.end())));
            while(it != const_dm_trials.end()) {
                ++count;
                ++it;
            }
            ASSERT_EQ(count, n);
        }

        // cbegin cend on const object
        {
            count=0;
            auto it = const_dm_trials.cbegin();
            ASSERT_EQ(n, std::size_t(std::distance(it, dm_trials.cend())));
            while(it != const_dm_trials.cend()) {
                ASSERT_EQ(it->size(), std::size_t((size)/std::pow(2, count)) ) << count;
                ++it;
                ++count;
            }
            ASSERT_EQ(count, n);
        }
    }
    */
}

TYPED_TEST_P(DmTrialsTester, host_conversion)
{
    /*
    typedef TypeParam Traits;
    typedef typename Traits::DmTrialsType DmTrialsType;
    typedef typename DmTrialsType::DmType Dm;
    typedef typename DmTrialsType::ValueType ValueType;

    auto now = cheetah::utils::ModifiedJulianClock::now();
    std::size_t size=1<<4;
    auto metadata = data::DmTrialsMetadata::make_shared(TimeType(0.000064*data::seconds), size);
    metadata->emplace_back(Dm(0.0*data::parsecs_per_cube_cm),1);
    metadata->emplace_back(Dm(10.0*data::parsecs_per_cube_cm),2);
    metadata->emplace_back(Dm(20.0*data::parsecs_per_cube_cm),4);

    typedef typename DmTrialsType::ValueType ValueType;
    typedef DmTrials<panda::Cpu, ValueType, std::allocator<ValueType>> HostDmTrialsType;

    // some host based data
    HostDmTrialsType host_data(metadata, now, std::allocator<ValueType>());
    for(auto& trial : host_data) {
        std::iota(trial.begin(), trial.end(), 1);
    }

    Traits traits;
    for(auto& device : this->_system.devices()) {
        // copy host data to target DmTrials
        DmTrialsType dm_trials(host_data, traits.allocator(*device));
        ASSERT_EQ(dm_trials.size(), host_data.size());
        // copy back from target to host
        HostDmTrialsType host_data_copy(dm_trials);
        ASSERT_EQ(host_data_copy.size(), metadata->size());
        auto copy_it=host_data_copy.cbegin();
        auto it=host_data.cbegin();
        while(it!=host_data.cend()) {
            ASSERT_EQ(it->size(), copy_it->size());
            auto copy_trial_it=copy_it->begin();
            for(auto const& v : *it) {
                ASSERT_EQ(v, *copy_trial_it);
                ++copy_trial_it;
            }
            ++it;
            ++copy_it;
        }
    }
    */
}

REGISTER_TYPED_TEST_SUITE_P(DmTrialsTester, construct_nullptr_metadata, construct_empty_metadata, begin_end, host_conversion);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
