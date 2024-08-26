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
#ifndef SKA_CHEETAH_DATA_TEST_UTILS_DMTRIALSTESTER_H
#define SKA_CHEETAH_DATA_TEST_UTILS_DMTRIALSTESTER_H

#include "cheetah/data/DmTrials.h"
#include <panda/test/TestDevice.h>
#include <gtest/gtest.h>

namespace ska {
namespace cheetah {
namespace data {
namespace test {

/**
 * @brief class to test the DmTrials class
 * @details
 */

template<typename DmTrialsT>
class DmTrialsTesterTraits
{
    public:
        typedef DmTrialsT DmTrialsType;
        typedef typename DmTrialsType::Architecture Arch;
        typedef typename DmTrialsType::Allocator Allocator;

    public:
        virtual ~DmTrialsTesterTraits() {}
        // override this function for specialist Allocators
        virtual Allocator allocator(panda::PoolResource<Arch>& device) = 0; // { return Allocator(device); }
};

template<typename DmTrialsTesterTraitsT>
class DmTrialsTester : public ::testing::TestWithParam<DmTrialsTesterTraitsT>
{
        typedef ::testing::TestWithParam<DmTrialsTesterTraitsT> BaseT;

    public:
        DmTrialsTester();

    protected:
        panda::test::TestDevice<typename DmTrialsTesterTraitsT::Arch> _system;
};

TYPED_TEST_SUITE_P(DmTrialsTester);

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
#include "detail/DmTrialsTester.cpp"

#endif // SKA_CHEETAH_DATA_TEST_UTILS_DMTRIALSTESTER_H
