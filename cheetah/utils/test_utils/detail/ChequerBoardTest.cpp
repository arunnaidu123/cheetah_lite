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
#include "cheetah/utils/test_utils/ChequerBoardTest.h"


namespace ska {
namespace cheetah {
namespace utils {
namespace test {

template <typename TestTraits>
ChequerBoardTest<TestTraits>::ChequerBoardTest()
    : cheetah::utils::test::AlgorithmTester<TestTraits>()
{
}

template <typename TestTraits>
ChequerBoardTest<TestTraits>::~ChequerBoardTest()
{
}

template<typename TestTraits>
void ChequerBoardTest<TestTraits>::SetUp()
{
}

template <typename TestTraits>
void ChequerBoardTest<TestTraits>::TearDown()
{
}

template<typename Device>
void apply_chequerboard(Device&, std::size_t)
{
    FAIL() << "No apply_chequerboard implementation for device";
}

template <typename DataType>
static bool verify_chequerboard(DataType const& data)
{
    auto it = data.begin();
    unsigned ii=0;
    while(it != data.end())
	{
	    if (*it != (ii++%2 == 0))
		return false;
	    ++it;
	}
    return true;
}

ALGORITHM_TYPED_TEST_P(ChequerBoardTest, test_chequerboard)
{
    // passed accelerator device to use as argument named device
    for (unsigned ii=1; ii<9999; ii+=123) {
        auto d_data = TypeParam::apply_chequerboard(device, ii);
        ASSERT_TRUE(verify_chequerboard<decltype(d_data)>(d_data)) << "size=" << ii;
    }
}

// each test defined by ALGORITHM_TYPED_TEST_P must be added to the
// test register (each one as an element of the comma seperated list)
REGISTER_TYPED_TEST_SUITE_P(ChequerBoardTest, test_chequerboard);

} // namespace test
} // namespace utils
} // namespace cheetah
} // namespace ska
