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
#ifndef SKA_CHEETAH_GENERATORS_GENERATORFACTORY_H
#define SKA_CHEETAH_GENERATORS_GENERATORFACTORY_H

#include "cheetah/generators/DataGenerator.h"
#include "cheetah/generators/Config.h"
#include "cheetah/generators/pulse_profile/ProfileManager.h"
#include "panda/Factory.h"

namespace ska {
namespace cheetah {
namespace generators {
/**
 * @brief
 *    construct a suitable generator object
 *
 * @details
 *
 */

template<typename DataType>
class GeneratorFactory : public ska::panda::Factory<DataGenerator<DataType>>
{
    public:
        GeneratorFactory(Config const&);
        ~GeneratorFactory();

    private:
        Config const& _config;
        ProfileManager _profile_manager;
        typename Config::PulsarInjectionIterartorType _pulsar_injection_it;
};


} // namespace generators
} // namespace cheetah
} // namespace ska
#include "detail/GeneratorFactory.cpp"

#endif // SKA_CHEETAH_GENERATORS_GENERATORFACTORY_H
