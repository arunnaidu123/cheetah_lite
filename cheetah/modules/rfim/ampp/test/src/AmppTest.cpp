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
#include "cheetah/modules/rfim/ampp/test/AmppTest.h"
#include "cheetah/modules/rfim/ampp/Rfim.h"
#include "cheetah/modules/rfim/DefaultRfimTraits.h"
#include "cheetah/modules/rfim/test_utils/RfimTester.h"
#include <memory>


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace test {

template<typename NumericalRepT>
struct AmppTestRfimTraits : public rfim::DefaultRfimTraits<data::TimeFrequency<Cpu, NumericalRepT>>
{
    typedef NumericalRepT NumericalRep;
    typedef rfim::DefaultRfimTraits<data::TimeFrequency<Cpu, NumericalRep>> BaseT;
    typedef data::TimeFrequency<Cpu, NumericalRep> TimeFrequencyType;
    typedef rfim::ampp::Spectrum<NumericalRep> BandPassType;
    using typename BaseT::Policy;
    using typename BaseT::BandPassHandler;
};

struct AmppTraits : public RfimTesterTraits<uint8_t, rfim::ampp::Rfim<AmppTestRfimTraits<uint8_t>>::Architecture> {

    private:
        typedef uint8_t NumericalRep;
        typedef RfimTesterTraits<NumericalRep, typename rfim::ampp::Rfim<AmppTestRfimTraits<NumericalRep>>::Architecture> BaseT;

    public:
        AmppTraits();

        ResultType apply_algorithm(DeviceType&, DataType& data);

        // return the api for the ampp algorithm
        static rfim::ampp::Rfim<AmppTestRfimTraits<NumericalRep>>& algo();

        // return the configuration for this api
        static rfim::ampp::Config& config();

    protected:
        bool _initial;

};

AmppTraits::AmppTraits()
    : _initial(true)
{
    // initialise the object
}


rfim::ampp::Config& AmppTraits::config()
{
    static rfim::ampp::Config config;
    auto& bp_config=config.bandpass();
    bp_config.mean(0.0);

    return config;
}

rfim::ampp::Rfim<AmppTestRfimTraits<typename AmppTraits::NumericalRep>>& AmppTraits::algo()
{
    static rfim::ampp::Rfim<AmppTestRfimTraits<NumericalRep>> algo_api(config(), [](typename AmppTestRfimTraits<NumericalRep>::BandPassType const&){});
    return algo_api;
}

AmppTraits::ResultType AmppTraits::apply_algorithm(DeviceType&, DataType& data)
{
    if(_initial) {
        std::size_t samples=0U;
        while(samples < 16384) {
            std::shared_ptr<DataType> data_copy(new DataType(data));
            algo().run(*data_copy); // send a copy of data to allow generation of model
            samples += data.dimension<data::Time>();
        }
        _initial = false;
    }
    return algo().run(data);
}

AmppTest::AmppTest()
    : ::testing::Test()
{
}

AmppTest::~AmppTest()
{
}

void AmppTest::SetUp()
{
}

void AmppTest::TearDown()
{
}

typedef ::testing::Types<AmppTraits> AmppTraitsTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(Ampp, RfimTester, AmppTraitsTypes);

} // namespace test
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
