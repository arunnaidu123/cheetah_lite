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
#include "cheetah/generators/SimplePhaseModelConfig.h"


namespace ska {
namespace cheetah {
namespace generators {


SimplePhaseModelConfig::SimplePhaseModelConfig()
    : utils::Config("simple_phase_model")
{
}

SimplePhaseModelConfig& SimplePhaseModelConfig::operator=(SimplePhaseModelConfig const& config)
{
    _coeffs = config._coeffs;
    return *this;
}

SimplePhaseModelConfig::~SimplePhaseModelConfig()
{
}

void SimplePhaseModelConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("coeffs", boost::program_options::value<panda::CommaSeparatedOptions<float>>(&_coeffs), "comma separated list of coefficients in the taylor expansion, each corresponding to progressively higher derivatives of phase by time")
    ;
}

std::vector<float> const& SimplePhaseModelConfig::coefficients() const
{
    return _coeffs;
}

void SimplePhaseModelConfig::coefficients(std::vector<float> coeffs)
{
    _coeffs = coeffs;
}

} // namespace generators
} // namespace cheetah
} // namespace ska
