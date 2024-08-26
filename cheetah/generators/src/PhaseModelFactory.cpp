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
#include "cheetah/generators/PhaseModelFactory.h"
#include "cheetah/generators/src/SimplePhaseModel.h"
#include "cheetah/generators/src/Tempo2PhaseModel.h"
#include "cheetah/generators/PulsarInjectionConfig.h"
#include <panda/Error.h>

namespace ska {
namespace cheetah {
namespace generators {


PhaseModelFactory::PhaseModelFactory(ConfigType const& config)
    : _iterators(config)
    , _types({"simple", "tempo2"})
{
}

PhaseModelFactory::~PhaseModelFactory()
{
}

PhaseModelFactory::type PhaseModelFactory::create(std::string const& type) const
{
    if(type=="simple") {
        return SimplePhaseModel(static_cast<SimplePhaseModelConfig const&>(_iterators.config("simple_phase_model")));
    }
    else if(type=="tempo2") {
        return Tempo2PhaseModel(static_cast<Tempo2PhaseModelConfig const&>(_iterators.config("tempo2_phase_model")));
    }
    else {
        panda::Error e("phase_model: unknown phase model type requested:");
        e << type;
        throw e;
    }
}

std::vector<std::string> const&  PhaseModelFactory::types() const
{
    return _types;
}
} // namespace generators
} // namespace cheetah
} // namespace ska
