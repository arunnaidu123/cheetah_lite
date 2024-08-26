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
#include "cheetah/modules/sps/test_utils/SpsTester.h"
#include "cheetah/modules/sps/detail/CommonTypes.h"
#include <memory>
#include <vector>

namespace ska {
namespace cheetah {
namespace modules {
namespace sps {
namespace klotski {
namespace test {

typedef sps::CommonTypes<sps::Config, uint8_t> SpsTraits;

struct KlotskiSpsTraits : public sps::test::SpsTesterTraits<sps::klotski::Sps<SpsTraits>::Architecture
                                                                ,sps::klotski::Sps<SpsTraits>::Architecture>
{
    typedef sps::klotski::Sps<SpsTraits> SpsType;
    typedef sps::test::SpsTesterTraits<typename SpsType::Architecture, typename SpsType::Architecture> BaseT;
    typedef typename BaseT::Arch Arch;
    void configure(sps::Config& config) override {
        BaseT::configure(config);
        auto& klotski_config = config.klotski_config();
        klotski_config.activate(true);
    }
};

} // namespace test
} // namespace klotski
} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska

namespace ska {
namespace cheetah {
namespace modules {
namespace sps {
namespace test {

typedef ::testing::Types<sps::klotski::test::KlotskiSpsTraits> KlotskiSpsTraitsTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(Klotski, SpsTester, KlotskiSpsTraitsTypes);

} // namespace test
} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska
