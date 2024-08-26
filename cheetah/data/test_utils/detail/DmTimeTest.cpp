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
#include "cheetah/data/test_utils/DmTimeTest.h"
#include "cheetah/data/test_utils/DmTrialsGeneratorUtil.h"
#include "cheetah/data/DmTime.h"
#include "cheetah/data/DmTrialsMetadata.h"
#include "cheetah/data/DmTrials.h"
#include "cheetah/data/TimeSeries.h"
#include "panda/Log.h"
#include <chrono>

namespace ska {
namespace cheetah {
namespace data {
namespace test {

template <typename Traits>
DmTimeTest<Traits>::DmTimeTest()
    : ::testing::Test()
{
}

template <typename Traits>
DmTimeTest<Traits>::~DmTimeTest()
{
}

template <typename Traits>
void DmTimeTest<Traits>::SetUp()
{
}

template <typename Traits>
void DmTimeTest<Traits>::TearDown()
{
}

template <typename Arch, typename T>
void Tester<Arch,T>::slice_test(std::size_t dms_per_slice, std::size_t ndms, std::size_t nblocks)
{
    typedef typename utils::ModifiedJulianClock::time_point StartTime;
    typedef data::DmTrials<Arch,T> DmTrialsType;
    typedef typename DmTrialsType::TimeType TimeType;
    typedef typename DmTrialsType::DmType Dm;
    auto buffer = DmTime<DmTrialsType>::make_shared();
    DmTrialsGeneratorUtil<DmTrialsType> trials_generator;

    for (std::size_t block_idx=0; block_idx<nblocks; ++block_idx)
    {
        buffer->add(trials_generator.generate(TimeType(0.1*data::seconds),10,ndms));
    }

    std::size_t slice_count = 0;
    std::size_t dm_count = 0;

    for (auto it = buffer->begin(dms_per_slice); it < buffer->end();  ++it )
    {
        auto slice = *it;
        for (auto slice_it = slice->begin(); slice_it != slice->end();  ++slice_it )
        {
            ++dm_count;
        }
        ++slice_count;
    }
    int remainder = (ndms % dms_per_slice != 0);
    ASSERT_EQ(slice_count,ndms/dms_per_slice + remainder);
    ASSERT_EQ(dm_count,ndms);
}

template <typename Arch, typename T>
void Tester<Arch,T>::add_test()
{
    typedef typename utils::ModifiedJulianClock::time_point StartTime;
    typedef data::DmTrials<Arch,T> DmTrialsType;
    typedef typename DmTrialsType::TimeType TimeType;
    typedef typename DmTrialsType::DmType Dm;
    auto buffer = DmTime<DmTrialsType>::make_shared();
    DmTrialsGeneratorUtil<DmTrialsType> trials_generator;
    StartTime epoch(utils::julian_day(50000.0));
    trials_generator.epoch(epoch);
    buffer->add(trials_generator.generate(TimeType(0.1*data::seconds),10,10));
    trials_generator.epoch(epoch+std::chrono::microseconds(12312));
    buffer->add(trials_generator.generate(TimeType(0.1*data::seconds),10,10));
    trials_generator.epoch(epoch+std::chrono::microseconds(12));
    buffer->add(trials_generator.generate(TimeType(0.1*data::seconds),10,10));
    trials_generator.epoch(epoch+std::chrono::microseconds(333));
    buffer->add(trials_generator.generate(TimeType(0.1*data::seconds),10,10));
    trials_generator.epoch(epoch+std::chrono::microseconds(111));
    buffer->add(trials_generator.generate(TimeType(0.1*data::seconds),10,10));
    trials_generator.epoch(epoch+std::chrono::microseconds(1000));
    buffer->add(trials_generator.generate(TimeType(0.1*data::seconds),10,10));

    auto& blocks = buffer->blocks();
    auto prev = blocks.begin();
    auto next = blocks.begin();
    ++next;
    while (next != blocks.end())
    {
        ASSERT_TRUE((*prev)->start_time() < (*next)->start_time());
        ++prev;
        ++next;
    }
}

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
