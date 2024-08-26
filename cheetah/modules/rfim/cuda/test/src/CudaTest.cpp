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
#include "cheetah/modules/rfim/cuda/test/CudaTest.h"
#include "cheetah/modules/rfim/cuda/Rfim.h"
#include "cheetah/modules/rfim/test_utils/RfimTester.h"
#include "cheetah/Configuration.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace test {


CudaTest::CudaTest()
    : ::testing::Test()
{
}

CudaTest::~CudaTest()
{
}

void CudaTest::SetUp()
{
}

void CudaTest::TearDown()
{
}

/* white box testing stuff here (no device available)
TEST_F(CudaTest, test_something)
{
}
*/

// make sure this represents the minimum requirements
#ifdef SKA_CHEETAH_ENABLE_CUDA
struct CudaTraits : public RfimTesterTraits<rfim::cuda::Rfim::Architecture, rfim::cuda::Rfim::ArchitectureCapabilty> {
    typedef RfimTesterTraits<rfim::cuda::Rfim::Architecture> BaseT;

    static ResultType& apply_algorithm(DeviceType& device, DataType& data)
    {
        static rfim::cuda::Config config;
        static rfim::cuda::Rfim algo(config);
        algo(device, data.shared_from_this());
        return data;
    }
};

typedef ::testing::Types<CudaTraits> CudaTraitsTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(Cuda, RfimTester, CudaTraitsTypes);
#endif // SKA_CHEETAH_ENABLE_CUDA

} // namespace test
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
