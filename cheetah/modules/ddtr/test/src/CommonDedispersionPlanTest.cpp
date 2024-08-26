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
#include "cheetah/modules/ddtr/test/CommonDedispersionPlanTest.h"
#include "cheetah/modules/ddtr/detail/CommonDedispersionPlan.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace test {


CommonDedispersionPlanTest::CommonDedispersionPlanTest()
    : ::testing::Test()
{
}

CommonDedispersionPlanTest::~CommonDedispersionPlanTest()
{
}

void CommonDedispersionPlanTest::SetUp()
{
}

void CommonDedispersionPlanTest::TearDown()
{
}

TEST_F(CommonDedispersionPlanTest, test_has_type)
{

    typedef CommonDedispersionPlan<TestTraits, AlgoA> PlanA;
    static_assert(PlanA::has_algo<AlgoA>(), "Expected to have Algorithm");

    static_assert(!PlanA::has_algo<AlgoB>(), "Expected not to have Algorithm");

    typedef CommonDedispersionPlanFilter<PlanA, AlgoA> FilterPlanA;
    static_assert(FilterPlanA::has_algo<AlgoA>(), "Expected to have Algorithm");

    static_assert(!FilterPlanA::has_algo<AlgoB>(), "Expected not to have Algorithm");
}

} // namespace test
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
