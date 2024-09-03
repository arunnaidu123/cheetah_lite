/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 The SKA organisation
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
#include "cheetah/data/DedispersionMeasure.h"
#include "cheetah/modules/spdt/test_utils/SpdtTester.h"
#include "cheetah/modules/spdt/detail/CommonTypes.h"
#include <memory>
#include <vector>
/*
namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {
namespace cpu {
namespace test {

typedef spdt::CommonTypes<spdt::Config, uint8_t> SpdtTraits;

struct CpuSpdtTraits : public spdt::test::SpdtTesterTraits<spdt::cpu::Spdt<SpdtTraits>::Architecture
                                                                ,spdt::cpu::Spdt<SpdtTraits>::Architecture>
{
    typedef spdt::cpu::Spdt<SpdtTraits> SpdtType;
    typedef spdt::test::SpdtTesterTraits<typename SpdtType::Architecture, typename SpdtType::Architecture> BaseT;
    typedef typename BaseT::Arch Arch;
    void configure(spdt::Config& config) override {
        BaseT::configure(config);
        auto& cpu_config = config.cpu_config();
        cpu_config.activate();
    }
};

} // namespace test
} // namespace cpu
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska

namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {
namespace test {

typedef ::testing::Types<spdt::cpu::test::CpuSpdtTraits> CpuSpdtTraitsTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(Cpu, SpdtTester, CpuSpdtTraitsTypes);

} // namespace test
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska
*/