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
#ifndef SKA_CHEETAH_UTILS_CONFIG_H
#define SKA_CHEETAH_UTILS_CONFIG_H

#include "cheetah/utils/System.h"
#include "panda/ConfigModule.h"
#include "panda/PoolSelector.h"

namespace ska {
namespace cheetah {
namespace utils {

/**
 * @brief
 *    Base class for module configuration
 *
 * @details
 *
 */
class Config : public panda::ConfigModule
{
    public:
        /**
         * @brief the configured panda::System object
         *        that matches the devices the user has selected to be
         *        compiled in (with the -DENABLE_XXX flags passed to cmake)
         */
        typedef System SystemType;

        /**
         * @brief the type of panda resource pool manager that can handle
         *        the full compiled in device support
         */
        typedef panda::PoolManager<SystemType> PoolManagerType;

        /**
         * @brief the type of panda resource pool that can handle
         *        the full compiled in device support
         */
        typedef typename PoolManagerType::PoolType PoolType;

    public:
        /**
         * @param module_name the configuration option name (xml tag) for the module node
         */
        Config(std::string module_name);

        /**
         * @brief call for notifiers to indicate a termination condition
         * @details will cause an exception to be thrown
         */
        void terminate() const;

    protected:

};

/**
 * @brief set the type of pool manager that will be usd in cheetah pipelines
 */

} // namespace utils
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_UTILS_CONFIG_H
