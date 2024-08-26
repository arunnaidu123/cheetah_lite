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
#ifndef SKA_CHEETAH_MODULES_DDTR_TEST_COMMONDEDISPERSIONPLANTEST_H
#define SKA_CHEETAH_MODULES_DDTR_TEST_COMMONDEDISPERSIONPLANTEST_H

#include "cheetah/data/TimeFrequency.h"
#include <gtest/gtest.h>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace test {

/**
 * @brief
 * @details
 */

class CommonDedispersionPlanTest : public ::testing::Test
{
    protected:
        void SetUp() override;
        void TearDown() override;

    public:
        CommonDedispersionPlanTest();

        ~CommonDedispersionPlanTest();

    protected:


        struct ArchA {};

        struct ArchB {};

        struct DedispersionPlanA {};

        struct DedispersionPlanB {};

        struct AlgoAConfig {};

        struct AlgoBConfig {};

        struct AlgoA{
            typedef ArchA Architecture;
            typedef DedispersionPlanA DedispersionPlan;
            typedef AlgoAConfig Config;
        };

        struct AlgoB{
            typedef ArchA Architecture;
            typedef DedispersionPlanB DedispersionPlan;
            typedef AlgoBConfig Config;
        };

        struct TestTraits
        {
            //typedef NumericalT NumericalRep;
            typedef uint8_t NumericalRep;
            typedef data::TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
            typedef typename AlgoA::Config Config;
        };

    private:
};


} // namespace test
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_DDTR_TEST_COMMONDEDISPERSIONPLANTEST_H
