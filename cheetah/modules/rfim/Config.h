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
#ifndef SKA_CHEETAH_MODULES_RFIM_CONFIG_H
#define SKA_CHEETAH_MODULES_RFIM_CONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/modules/rfim/ampp/Config.h"
#include "cheetah/modules/rfim/cuda/Config.h"
#include "cheetah/modules/rfim/sum_threshold/Config.h"
#include "cheetah/modules/rfim/iqrmcpu/Config.h"
#include "panda/arch/nvidia/Nvidia.h"
#include "panda/PoolSelector.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {

/**
 * @brief
 *    Configuration Parameters for the RFIM module
 * @details
 *
 */
class Config : public cheetah::utils::Config
{
    public:
        static const unsigned MaxPriority = 2;

    public:
        Config();
        ~Config();

        /**
         * @brief Configuration details for the AMPP RFIM algorithm
         */
        ampp::Config const& ampp_algo_config() const;

        /**
         * @brief Configuration details for the cuda based RFIM algorithm
         */
        cuda::Config const& cuda_algo_config() const;

        /**
         * @brief Configuration details for the sum_threshold based RFIM algorithm
         */
        sum_threshold::Config const& sum_threshold_algo_config() const;

        /**
         * @brief Configuration details for the iqrm RFIM algorithm
         */
        iqrmcpu::Config const& iqrmcpu_algo_config() const;

        /**
         * @brief returns true only if one or more algorithms have been marked as active
         */
        bool algo_defined() const;

        /**
         * @brief data policy
         * @details specifies the rfim::Policy type to use
         *          if true the it will use rfim::FlagPolicy
         *          else will use rfim::LastUnflaged which modifies the input data
         */
        bool flag_policy() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        ampp::Config _ampp_config;
        cuda::Config _cuda_config;
        sum_threshold::Config _sum_threshold_config;
        iqrmcpu::Config _iqrmcpu_config;
        bool _policy;
};

// defines a templated ConfigType for Rfim that adds on pool selection
template<typename PoolManagerType>
using ConfigType = panda::PoolSelector<PoolManagerType, Config>;


} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_RFIM_CONFIG_H
