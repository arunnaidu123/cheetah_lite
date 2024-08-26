/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 The SKA organisation
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

#include "cheetah/modules/ddtr/test_utils/DdtrTester.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace cpu {
namespace test {

template <typename NumericalT>
struct DdtrCpuTraits : public ddtr::test::DdtrTesterTraits<ddtr::cpu::Ddtr, NumericalT>
{
    typedef ddtr::test::DdtrTesterTraits<ddtr::cpu::Ddtr, NumericalT> BaseT;
    typedef ddtr::DedispersionConfig::Dm Dm;
    typedef typename BaseT::Arch Arch;
    void configure(ddtr::Config& config) override {
        config.cpu_algo_config().active(true);
        config.dedispersion_samples(1<<14);
        PANDA_LOG << "Number of added dm trials: " << config.dm_trials().size();
    }
    private:
        std::vector<std::unique_ptr<ddtr::DedispersionConfig>> _configs; // keep configs in scope
};

} // namespace test
} // namespace cpu
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace test {

typedef ::testing::Types<ddtr::cpu::test::DdtrCpuTraits<uint8_t>> DdtrCpuTraitsTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(Cpu, DdtrTester, DdtrCpuTraitsTypes);

} // namespace test
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
