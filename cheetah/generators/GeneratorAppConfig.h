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
#ifndef SKA_CHEETAH_GENERATOR_GENERATORAPPCONFIG_H
#define SKA_CHEETAH_GENERATOR_GENERATORAPPCONFIG_H

#include "panda/BasicAppConfig.h"
#include "cheetah/generators/Config.h"
#include "cheetah/sigproc/WriterConfig.h"
#include "cheetah/data/Units.h"
#include <string>
#include <vector>

namespace ska {
namespace cheetah {
namespace generators {

/**
 * @brief
 *   Top level configuration for the generator app
 *
 * @details
 *
 */

class GeneratorAppConfig : public ska::panda::BasicAppConfig
{
        typedef boost::units::quantity<data::MegaHertz, double> FrequencyType;
        typedef boost::units::quantity<boost::units::si::time, double> IntervalType;

    public:
        GeneratorAppConfig(std::string const& app_name, std::string const& description);
        ~GeneratorAppConfig();

        /**
         * @brief set the known generators
         */
        void set_generator_list(std::vector<std::string> const& generator_names);

        /**
         * @brief list the known generators
         */
        std::vector<std::string> const& generator_list() const;

        /**
         * @brief return the generator configuration object
         */
        ska::cheetah::generators::Config& generator_config();

        /**
         * @brief return the configuration object for directing sigproc writer
         */
        sigproc::WriterConfig const& sigproc_config() const;

        /**
         * @brief return the number of data chunks to produce for the data file
         */
        std::size_t number_of_chunks() const;

        /**
         * @brief return the number of time smaples per data chunk
         */
        std::size_t number_of_time_samples() const;

        /**
         * @brief return the number of channels for each data chunk
         */
        std::size_t number_of_channels() const;

        /**
         * @brief return the chosen data model
         */
        std::vector<std::string> const& data_generator() const;

        /**
         * @brief return the start frequency
         */
        FrequencyType start_frequency() const;

        /**
         * @brief return the channel width (homgenous cwidth channels)
         */
        FrequencyType channel_width() const;

        /**
         * @brief sample interval (in seconds)
         */
        IntervalType sample_interval() const;


    protected:
        std::string version() const override;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        ska::cheetah::generators::Config _generator_config;
        sigproc::WriterConfig _sigproc_config;
        std::vector<std::string> _generator_keys;
        std::size_t _number_of_chunks;
        std::size_t _number_of_channels;
        std::size_t _number_of_time_samples;
        std::vector<std::string> _generator_selected;
        FrequencyType _frequency;
        FrequencyType _channel_width;
        IntervalType _sample_interval;
};


} // namespace generators
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_GENERATOR_GENERATORAPPCONFIG_H
