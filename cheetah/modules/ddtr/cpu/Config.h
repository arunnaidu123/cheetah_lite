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
#ifndef SKA_CHEETAH_MODULES_DDTR_CPU_CONFIG_H
#define SKA_CHEETAH_MODULES_DDTR_CPU_CONFIG_H

#include "cheetah/utils/Config.h"
#include "panda/PoolSelector.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace cpu {

/**
 * @brief  configuration parameters for the ddtr cpu module
 *
 */

class Config : public utils::Config
{
    public:
        Config();
        ~Config();

        /**
         * @brief return true if this algorithm is to be used
         */
        bool active() const;

         /**
         * @brief set the activation value
         */
        void active(bool);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        bool _active;
};

// defines a templated ConfigType for Ddtr that adds on pool selection
template<typename PoolManagerType>
using ConfigType = panda::PoolSelector<PoolManagerType, Config>;

} // namespace cpu
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_DDTR_CPU_CONFIG_H
