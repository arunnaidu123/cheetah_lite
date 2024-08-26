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
#ifndef SKA_CHEETAH_EMULATOR_EMULATORAPPCONFIG_H
#define SKA_CHEETAH_EMULATOR_EMULATORAPPCONFIG_H

#include "panda/BasicAppConfig.h"
#include "cheetah/emulator/Config.h"
#include <string>
#include <vector>
#include <functional>

namespace ska {
namespace cheetah {
namespace emulator {

/**
 * @brief
 *   Top level configuration for the emulator app
 *
 * @details
 *
 */

class EmulatorAppConfig : public ska::panda::BasicAppConfig
{
    public:
        /**
         * @brief Configuration for an Emulation app to allow selection if streams, generators etc
         * @param stream_factory an object for creating and executing the streams at the top level of the app
         *                       This object must support the following methods:
         *                         std::vector<std::string> available(); // list of avaialble streams to select from
         *                         bool print_generators(emulator::Config const&); // callback to be executed when --list-generators is called
         */
        template<typename StreamFactoryT>
        EmulatorAppConfig(std::string const& app_name, std::string const& description
                        , StreamFactoryT stream_factory);
        EmulatorAppConfig(EmulatorAppConfig const&) = delete;
        ~EmulatorAppConfig();

        /**
         * @brief set the known generators
         */
        void set_generator_list(std::vector<std::string> const& generator_names);

        /**
         * @brief list the known generators
         */
        std::vector<std::string> const& generator_list() const;

        ska::cheetah::emulator::Config& emulator_config();
        ska::cheetah::emulator::Config const& emulator_config() const;

        /**
         * @brief the type of UDP stream to generate
         */
        std::string const& stream_type() const;

    protected:
        std::string version() const override;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        typedef std::function<void(EmulatorAppConfig const&)> ListTypeCallback;

    private:
        std::vector<std::string> _available_streams;
        ListTypeCallback _generator_types_callback;
        ska::cheetah::emulator::Config _emulator_config;
        std::string _stream_type;
};


} // namespace emulator
} // namespace cheetah
} // namespace ska
#include "cheetah/emulator/detail/EmulatorAppConfig.cpp"

#endif // SKA_CHEETAH_EMULATOR_EMULATORAPPCONFIG_H
