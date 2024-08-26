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
#include "cheetah/generators/GeneratorFactory.h"
#include "cheetah/generators/DataGenerator.h"
#include "cheetah/generators/DispersedPulse.h"
#include "cheetah/generators/GaussianNoise.h"
#include "cheetah/generators/BasebandGaussianNoise.h"
#include "cheetah/generators/PulsarInjection.h"
#include "cheetah/generators/RfiScenario.h"
#include "cheetah/generators/NullGenerator.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/FrequencyTime.h"
#include "panda/Log.h"
#include "panda/Error.h"


namespace ska {
namespace cheetah {
namespace generators {


template<typename DataType>
GeneratorFactory<DataType>::GeneratorFactory(Config const& config)
    : ska::panda::Factory<DataGenerator<DataType>>("GeneratorFactory:")
    , _config(config)
    , _pulsar_injection_it(config.pulsar_injection_end())
{
    this->add_type("gaussian_noise", [this]() { return new GaussianNoise<DataType>(_config.gaussian_noise()); });
    this->add_type("pulsar_injection", [this]() {
                                    if(_pulsar_injection_it == _config.pulsar_injection_end())
                                    {
                                        _pulsar_injection_it = _config.pulsar_injection_begin();
                                        if(_pulsar_injection_it == _config.pulsar_injection_end()) {
                                            throw panda::Error("pulsar_injection requested but no corresponding configuration available");
                                        }
                                    }
                                    return new PulsarInjection<DataType>(*_pulsar_injection_it++, _profile_manager);
                                 }
            );

    this->add_type("dispersed_pulse", [this]() { return new DispersedPulse<DataType>(_config.dispersed_pulse()); });
    this->add_type("RfiScenario0", []() { return new RfiScenario<0, DataType>(); });
    this->add_type("RfiScenario1", []() { return new RfiScenario<1, DataType>(); });
    this->add_type("RfiScenario2", []() { return new RfiScenario<2, DataType>(); });
    this->add_type("RfiScenario3", []() { return new RfiScenario<3, DataType>(); });
    this->add_type("RfiScenario4", []() { return new RfiScenario<4, DataType>(); });
    this->add_type("RfiScenario5", []() { return new RfiScenario<5, DataType>(); });
    this->add_type("null", []() { return new NullGenerator<DataType>(); });

}

template<typename DataType>
GeneratorFactory<DataType>::~GeneratorFactory()
{
}

} // namespace generators
} // namespace cheetah
} // namespace ska
