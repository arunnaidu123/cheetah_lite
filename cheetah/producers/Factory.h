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
#ifndef SKA_CHEETAH_PRODUCERS_FACTORY_H
#define SKA_CHEETAH_PRODUCERS_FACTORY_H

#include "cheetah/producers/Config.h"
#include <tuple>
#include <vector>

namespace ska {
namespace cheetah {
namespace producers {

template<typename DataType>
class Factory
{
    public:
        Factory(Config<DataType> const& source_config);

        /**
         * @brief return the available source types available
         */
        std::vector<std::string> available() const;

        /**
         * @brief Launch the provided pipeline using the source identified by stream_name
         */
        template<typename ComputeModule>
        int exec(std::string const& stream_name, ComputeModule& pipeline);
};

template<typename NumericalT>
class Factory<data::SpCandidateData<data::TimeFrequency<Cpu, NumericalT>>>
{
        typedef data::TimeFrequency<Cpu, NumericalT> TimeFrequencyType;
        typedef data::SpCandidateData<TimeFrequencyType> DataType;
        typedef Config<DataType> ConfigType;

    public:
        Factory(ConfigType const& producers_config);

        /**
         * @brief return the available source types available
         */
        std::vector<std::string> available() const;

        /**
         * @brief Launch the provided pipeline using the source identified by stream_name
         */
        template<typename ComputeModule>
        int exec(std::string const& stream_name, ComputeModule& pipeline);

   private:
        ConfigType const& _config;
};

} // namespace producers
} // namespace cheetah
} // namespace ska
#include "detail/Factory.cpp"

#endif // SKA_CHEETAH_PRODUCERS_FACTORY_H
