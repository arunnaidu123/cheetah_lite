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
#ifndef SKA_CHEETAH_GENERATORS_PHASEMODELFACTORY_H
#define SKA_CHEETAH_GENERATORS_PHASEMODELFACTORY_H


#include "cheetah/data/Units.h"
#include "cheetah/utils/Config.h"
#include "cheetah/utils/ModifiedJulianClock.h"
#include "panda/ConfigModuleIteratorSet.h"
#include <functional>

namespace ska {
namespace cheetah {
namespace generators {
class PulsarInjectionConfig;

/**
 * @brief
 *    Factory to generate known phase models
 *
 * @details
 */

class PhaseModelFactory
{
        typedef utils::Config ConfigType;

    public:
        typedef std::function<double(utils::ModifiedJulianClock::time_point const&, boost::units::quantity<data::MegaHertz, double> const)> type;

    public:
        PhaseModelFactory(ConfigType const&);
        ~PhaseModelFactory();

        /**
         * @brief creates a phase model of the specified type, using the next available suitable configuration that is found
         */
        type create(std::string const& type) const;

        /**
         * @brief return the types supported
         */
        std::vector<std::string> const& types() const;

    private:
        mutable panda::ConfigModuleIteratorSet _iterators;
        std::vector<std::string> _types;
};


} // namespace generators
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_GENERATORS_PHASEMODELFACTORY_H
