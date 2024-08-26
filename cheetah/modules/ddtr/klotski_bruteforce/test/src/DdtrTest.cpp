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
#include "cheetah/modules/ddtr/klotski_bruteforce/Ddtr.h"
#include "cheetah/modules/ddtr/test_utils/DdtrTester.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski_bruteforce {
namespace test {

template <typename NumericalT>
struct klotskiBruteforceTraits
    : public ddtr::test::DdtrTesterTraits<ddtr::klotski_bruteforce::Ddtr, NumericalT>
{
    typedef ddtr::test::DdtrTesterTraits<ddtr::klotski_bruteforce::Ddtr, NumericalT> BaseT;

    void configure(ddtr::Config& config) override {
        BaseT::configure(config);
        config.klotski_bruteforce_algo_config().active(true);
    }

    bool expected_to_pass(data::TimeFrequency<Cpu, NumericalT> const& data) override {
        if(data.number_of_channels()>8192 || data.number_of_channels()%4!=0) return false;
        return true;
    }
};

} // namespace test
} // namespace klotski_bruteforce
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace test {

/**
* @test generic tester for klotski_bruteforce
* @given klotski_bruteforce module
* @when run through the DdtrTester
* @then runs all the generic test algorithms
*/

typedef ::testing::Types< ddtr::klotski_bruteforce::test::klotskiBruteforceTraits<uint8_t>
                        , ddtr::klotski_bruteforce::test::klotskiBruteforceTraits<uint16_t>
                        , ddtr::klotski_bruteforce::test::klotskiBruteforceTraits<float>
                        > klotskiBruteforceTraitsTypes;

INSTANTIATE_TYPED_TEST_SUITE_P(Cpu, DdtrTester, klotskiBruteforceTraitsTypes);

} // namespace test
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
