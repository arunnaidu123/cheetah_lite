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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_CHEETAHCONFIG_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_CHEETAHCONFIG_H

#include "cheetah/pipelines/search_pipeline/MultiBeamConfig.h"
#include "cheetah/pipelines/search_pipeline/EmptyConfig.h"
#include "cheetah/pipelines/search_pipeline/ScanConfig.h"
#include "cheetah/channel_mask/ConfigurableChannelMaskConfig.h"
#include "cheetah/modules/ddtr/Config.h"
#include "cheetah/modules/rfim/Config.h"
#include "cheetah/modules/spdt/Config.h"
#include "cheetah/modules/sps_clustering/Config.h"
#include "cheetah/modules/spsift/Config.h"

#include "panda/ConfigModule.h"
#include "panda/PoolManagerConfig.h"
#include "panda/PoolSelector.h"
#include "panda/DataSwitchConfig.h"

#include <boost/program_options.hpp>

#include <cstdlib>
#include <string>
#include <vector>

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

/**
 * @brief
 *    Parse configuration parameters for a cheetah pipeline application
 *
 * @details
 *
 */

template<typename NumericalRep>
class CheetahConfig : public utils::Config
{
    public:
        typedef utils::Config::PoolManagerType PoolManagerType;

    public:
        CheetahConfig(std::string const& app_name = "cheetah");
        ~CheetahConfig();

        /**
         * @brief return the cheetah version as a string
         */
        std::string version() const;

        /**
         * @brief return the selected stream name
         */
        std::string const& stream_name() const;

        /**
         * @brief wether to activte timing between each invocation of the runtime computational pipeline
         */
        bool time_handler_invocation() const;

        /**
         * @brief return the selected pipeline name
         */
        std::string const& pipeline_name() const;

        /**
         * @brief set the computational unit names available
         */
        void set_pipeline_handlers(std::vector<std::string> const& handler_names);

        /**
         * @brief return the configuration node with beam configurations
         */
        MultiBeamConfig<NumericalRep> const& beams_config() const;

        /**
         * @brief return the channel_mask configuration
         */
        channel_mask::ConfigurableChannelMaskConfig<NumericalRep> const& channel_mask_config() const;
        channel_mask::ConfigurableChannelMaskConfig<NumericalRep>& channel_mask_config();

        /**
         * @brief return the fldo module specific configuration parameters
         */
        //modules::ddtr::ConfigType const& ddtr_config() const;
        //modules::ddtr::ConfigType& ddtr_config();

        /**
         * @brief return the rfim module specific configuration parameters
         */
        modules::rfim::ConfigType<PoolManagerType> const& rfim_config() const;

        /**
         * @brief return the spdt module specific configuration parameters
         */
        modules::ddtr::ConfigType<PoolManagerType> const& ddtr_config() const;
        modules::ddtr::ConfigType<PoolManagerType>& ddtr_config();

        /**
         * @brief return the spdt module specific configuration parameters
         */
        modules::spdt::ConfigType<PoolManagerType> const& spdt_config() const;
        modules::spdt::ConfigType<PoolManagerType>& spdt_config();


        /*
         * @brief return spsift module specific configuration parameters
         */
        modules::spsift::Config const& spsift_config() const;

        /*
         * @brief return sps_clustering module specific configuration parameters
         */
        modules::sps_clustering::Config const& sps_clustering_config() const;


        /**
         * @brief return the data switch configuration parameters
         */
        panda::DataSwitchConfig& switch_config() const;

        /**
         * @brief return the data switch configuration parameters
         */
        panda::PoolManagerConfig<utils::Config::SystemType> const& pool_manager_config() const;

        /**
         * @brief return the pool manager object
         */
        PoolManagerType const& pool_manager() const;
        PoolManagerType& pool_manager();

        /**
         * @brief return the empty module specific configuration parameters
         */
        EmptyConfig const& empty_config() const;

        /**
         * @brief return the empty module specific configuration parameters
         */
        ScanConfig const& scan_config() const;

        /**
         * @brief parse the command line options/config file
         * @returns 0 to indicate no parse halting options specified
         * @returns >0 to indicate parse failed or parse indicates program halt
         */
        int parse(int argc, char** argv);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        boost::program_options::options_description _desc;
        boost::program_options::options_description _all_desc;
        boost::program_options::positional_options_description _options_pod;

        std::string _app_name;
        utils::Config::SystemType& _system;
        std::string _stream_name;
        std::vector<std::string> _pipeline_handler_names;
        std::string _pipeline_name;
        bool _handler_timing;
        mutable panda::DataSwitchConfig _switch_config; // contains the thread pools used by other objects
                                                        // so should be destroyed after MultiBeamConfig

        typedef panda::PoolManagerConfig<utils::Config::SystemType> PoolManagerConfigType;
        PoolManagerConfigType _pool_manager_config;

        PoolManagerType _pool_manager;
        ScanConfig _scan_config;
        MultiBeamConfig<NumericalRep> _beam_config;

        channel_mask::ConfigurableChannelMaskConfig<NumericalRep> _channel_mask_config;
        //modules::ddtr::ConfigType _ddtr_config;
        modules::ddtr::ConfigType<PoolManagerType> _ddtr_config;
        modules::rfim::ConfigType<PoolManagerType> _rfim_config;
        modules::spdt::ConfigType<PoolManagerType> _spdt_config;
        modules::spsift::Config _spsift_config;
        modules::sps_clustering::Config _sps_clustering_config;
        EmptyConfig _empty_config;
};

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "cheetah/pipelines/search_pipeline/detail/CheetahConfig.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_CHEETAHCONFIG_H
