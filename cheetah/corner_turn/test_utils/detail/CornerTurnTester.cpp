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

#include "cheetah/corner_turn/test_utils/TestContainer.h"
#include "cheetah/corner_turn/test_utils/CornerTurnTester.h"
#include <random>

namespace ska {
namespace cheetah {
namespace corner_turn {
namespace test {


template <typename TestTraits>
CornerTurnTester<TestTraits>::CornerTurnTester()
{
}

template <class TypeParam>
void run_test(std::size_t first_dimension_samples, std::size_t second_dimension_samples)
{
    typedef typename TypeParam::SrcType SrcType;
    typedef typename TypeParam::DstType DstType;
    typedef typename TypeParam::Architecture Arch;


    std::size_t size = first_dimension_samples*second_dimension_samples;
    TestContainer<Arch, SrcType> src_container(size);
    TestContainer<Arch, DstType> dst_container(size);

    std::vector<SrcType> host_source(size);
    srand((unsigned) time(0));
    std::generate(host_source.begin(), host_source.end(), [&]() { return (rand()%255); });
    panda::copy(host_source.begin(), host_source.end(), src_container.begin());

    TypeParam executor;
    executor.corner_turn(src_container.begin()
                       , dst_container.begin()
                       , first_dimension_samples
                       , second_dimension_samples);

    std::vector<DstType> host_destin(size);
    panda::copy(dst_container.begin(), dst_container.end(), host_destin.begin());

    auto src_it = host_source.begin();
    auto dst_it = host_destin.begin();

    for(std::size_t i=0; i<second_dimension_samples; ++i)
    {
        for(std::size_t j=0; j<first_dimension_samples; ++j)
        {
            ASSERT_EQ(*(src_it+i*first_dimension_samples+j), *(dst_it+j*second_dimension_samples+i));
        }
    }
}

/**
* @test test_cornerturn_contiguous
* @given A set of contiguous memory arrays with 2 nominal dimensions defining a 2D matrix of various sizes
*        between 8 to 1024 elements in each dimension
* @when corner_turn is called with each array in the set in turn
* @then the data is transferred to the destination container in the corner turned ordering
*/
TYPED_TEST_P(CornerTurnTester, test_cornerturn_contiguous)
{
    // always test a range of fixed size arrays
    for(std::size_t s=128; s<1024; s+=128)
    {
        for(std::size_t c=128; c<1024; c+=128)
        {
            run_test<TypeParam>(c,s);
        }
    }

    // fixed sized array sizes are augmented by further random sized containers
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<std::size_t> nspectra(8, 1024);
    std::uniform_int_distribution<std::size_t> nchans(8, 1024);

    for(std::size_t i=0; i<10; ++i)
    {
        run_test<TypeParam>(nchans(mt), nspectra(mt));
    }

}

REGISTER_TYPED_TEST_SUITE_P(CornerTurnTester, test_cornerturn_contiguous);

} // namespace test
} // namespace CornerTurnTester
} // namespace cheetah
} // namespace ska
