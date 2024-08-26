/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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
#ifndef SKA_CHEETAH_UTILS_CONFIGMODULE_H
#define SKA_CHEETAH_UTILS_CONFIGMODULE_H

#include "cheetah/utils/Config.h
#include "panda/MultipleConfigModule.h"
#include "panda/PoolSelector.h"

namespace ska {
namespace cheetah {
namespace utils {

/**
 * @brief A base class that can be used for multi-algorithm asyncronous modules
 * @details Provides a ConfigNode complete with the @ref ska::panda::PoolSelector attributes for
 *          resource pool selection and getters/setters for each sub algorithm configuration.
 * @tparam AlgoConfigTs the Config classes for each algorithm in the module.
 * ~~~~{.cpp}
 * namespace my_module {
 *   typedef cheetah::utils::ConfigModule<my_module::algo_a::Config
 *                                       ,my_module::algo_b::Config
 *                                       > ConfigBase;
 *   class Config : ConfigBase
 *   {
 *       public:
 *           Config()
 *             : ConfigBase("my_module")   // <- the name of the config node/option
 *           {
 *               // add any extra panda::ConfigModule based nodes here
 *               // to build up your modules configuration tree
 *           }
 *
 *       protected:
 *           // if you add extra options by providing this function be
 *           // sure to call the base class add_options method inside
 *           void add_options(ConfigModule::OptionsDescriptionEasyInit&) override;
 *   };
 *
 *   class MyModule {
 *       public:
 *         MyModule(Config const& config)
 *            : _algo_a(config.template config<my_module::algo_a::Config>())
 *            , _algo_b(config.template config<my_module::algo_a::Config>())
 *         {
 *             // with the config object you can access the allocated pool e.g.
 *             auto pool = config.pool();
 *         }
 *
 *       private:
 *         my_module::algo_a::MyModule _algo_a;
 *         my_module::algo_b::MyModule _algo_b;
 *   };
 * } // namespace my_module
 * ~~~~
 */

template<class... AlgoConfigTs>
class ConfigModule : public panda::PoolSelector<typename Config::PoolManagerType,
                                panda::MultipleConfigModule<Config, AlgoConfigTs...>
                                >
{
    public:
        /**
         * @brief constructor
         * @param module_name The name of the module (snake_case) configuration tag
         */
        ConfigModule(std::string module_name);

        /**
         * @brief copy constructor not available
         * @internal if you add a copy constructor please remember "rule of 5" and unit tests
         */
        ConfigModule(ConfigModule const&) = delete;
};


} // namespace utils
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_UTILS_CONFIGMODULE_H
