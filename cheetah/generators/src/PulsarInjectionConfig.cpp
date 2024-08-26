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
#include "cheetah/generators/PulsarInjectionConfig.h"
#include <panda/Error.h>
#include <numeric>


namespace ska {
namespace cheetah {
namespace generators {


PulsarInjectionConfig::PulsarInjectionConfig(std::string const& tag_name)
    : utils::Config(tag_name)
    , _signal_to_noise(5)
    , _spectral_index(0)
    , _profile_name("B0011+47")
    , _phase_model_factory(*this)
    , _dm(50.0*data::parsecs_per_cube_cm)
{
    add(_simple_phase_model_config);
    add(_tempo2_phase_model_config);
}

PulsarInjectionConfig::~PulsarInjectionConfig()
{
}

void PulsarInjectionConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    std::vector<std::string> const& phase_model_types = _phase_model_factory.types();
    std::string const phase_model_type_help = "phase model type to use (valid types: "  + std::accumulate( std::next(phase_model_types.begin())
                                                              , phase_model_types.end()
                                                              , *phase_model_types.begin()
                                                              , [](const std::string& a, const std::string& b) {
                                                                    return a + ", " + b;
                                                                }
                                                              ) + ")";
    add_options
        ("profile", boost::program_options::value<std::string>()->notifier([&](std::string const& p) { _profile_name=static_cast<PulsarProfileId>(p); }), "The name of the pulsar profile to emulate")
        ("snr", boost::program_options::value<float>(&_signal_to_noise)->default_value(_signal_to_noise), "Target signal-to-noise ratio (phase average S/N)")
        ("spectral_index", boost::program_options::value<float>(&_spectral_index)->default_value(_spectral_index), "Target spectral index")
        ("dm", boost::program_options::value<double>()->default_value(_dm.value())->notifier(
                    [&](double dm) {
                        _dm = dm * data::parsecs_per_cube_cm;
                    }
               )
             ,"dedispersion measure (pc/cm^3)")
        ("phase_model", boost::program_options::value<std::string>()->default_value("simple")->notifier(
                            [&](std::string const type) {
                                phase_model(type);
                            }
                        )
                      , phase_model_type_help.c_str() )
    ;
}

float PulsarInjectionConfig::signal_to_noise() const
{
    return _signal_to_noise;
}

void PulsarInjectionConfig::signal_to_noise(float snr)
{
    _signal_to_noise = snr;
}

PulsarProfileId const& PulsarInjectionConfig::profile() const
{
    return _profile_name;
}

void PulsarInjectionConfig::profile(std::string const& profile_name)
{
    _profile_name = PulsarProfileId(profile_name);
}

void PulsarInjectionConfig::spectral_index(float si)
{
    _spectral_index = si;
}

float PulsarInjectionConfig::spectral_index() const
{
    return _spectral_index;
}

PulsarInjectionConfig::PhaseModelType const& PulsarInjectionConfig::phase_model() const
{
    if(!_phase_model) throw panda::Error("phase model not specified");
    return _phase_model;
}

void PulsarInjectionConfig::phase_model(std::string const& type)
{
    _phase_model=_phase_model_factory.create(type);
}

void PulsarInjectionConfig::set_phase_model(SimplePhaseModelConfig const& config)
{
    _simple_phase_model_config = config;
    phase_model("simple");
}

void PulsarInjectionConfig::set_phase_model(Tempo2PhaseModelConfig const& config)
{
    _tempo2_phase_model_config = config;
    phase_model("tempo2");
}

data::DedispersionMeasureType<double> PulsarInjectionConfig::dm() const
{
    return _dm;
}

void PulsarInjectionConfig::dm(data::DedispersionMeasureType<double> dm)
{
    _dm = dm;
}

} // namespace generators
} // namespace cheetah
} // namespace ska
