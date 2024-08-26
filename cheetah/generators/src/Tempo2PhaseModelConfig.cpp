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
#include "cheetah/generators/Tempo2PhaseModelConfig.h"


namespace ska {
namespace cheetah {
namespace generators {

Tempo2PhaseModelConfig::Tempo2PhaseModelConfig()
    : utils::Config("tempo2_phase_model")
{
}

Tempo2PhaseModelConfig::~Tempo2PhaseModelConfig()
{
}

Tempo2PhaseModelConfig& Tempo2PhaseModelConfig::operator=(Tempo2PhaseModelConfig const& config)
{
    _filename = config._filename;
    return *this;
}

void Tempo2PhaseModelConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("filename", boost::program_options::value<boost::filesystem::path>(&_filename), "tempo2 file containing the model parameters")
    ;
}

boost::filesystem::path const& Tempo2PhaseModelConfig::filename() const
{
    return _filename;
}

void Tempo2PhaseModelConfig::filename(boost::filesystem::path const& filename)
{
    _filename = filename;
}

} // namespace generators
} // namespace cheetah
} // namespace ska
