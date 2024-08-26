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
#ifndef SKA_CHEETAH_IO_EXPORTERS_DATAEXPORTCONFIG_H
#define SKA_CHEETAH_IO_EXPORTERS_DATAEXPORTCONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/io/exporters/DataExportStreamConfig.h"
#include "panda/ChannelsConfig.h"
#include "panda/DataSwitchConfig.h"
#include <vector>
#include <string>

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief
 *     Configuration Object for DataExport module
 * @details
 * @code
 *  <sinks>
 *    <threads/>
 *    <channels>
 *      <channel_1>
 *         <active>trus</active>
 *         <sink>sink_id_1</sink>
 *         <sink>sink_id_2</sink>
 *      </channel_1>
 *    </channels>
 *    <my_sink>
 *        <id>sink_id_1</id>
 *        <other_my_sink_config/>
 *    </my_sink>
 *    <other_sink>
 *        <id>sink_id_2</id>
 *    </other_sink>
 *  </sinks>
 * @endcode
 * Each sink is assigned to a channel using its id. The name of the sink
 * should mathc the ExporterType key in the DataExport factory (see set_factory() methdo)
 *
 */
class DataExportConfig : public utils::Config
{
    public:
        DataExportConfig(std::string const& tag = std::string("sinks") );
        ~DataExportConfig();

        /**
         * @brief return the configuration suitable for sending to a panda::DataSwitch
         */
        panda::DataSwitchConfig& switch_config() const;

        /**
         * @brief mark the provided streams as active
         */
        void activate_streams(std::vector<std::string> const& streams);
        void activate(panda::ChannelId const& channel);

        /**
         * @brief set the number of dedicated threads to service exporters export (default 0)
         * @details for finer control use the @ref set_engine_config() methods
         */
        void number_of_threads(unsigned const& number_of_threads);

        /**
         * @brief set an export streamer configurations
         */
        void add_exporter(DataExportStreamConfig);

        /**
         * @brief return a list of configured export streamer configurations
         */
        std::vector<DataExportStreamConfig> const& exporters() const;

        /**
         * @brief explicity add a channel configuration
         */
        void channel(panda::ChannelInfo const&);

         /**
         * @brief return the processing engine configuration associated with the specified channel
         */
        panda::ProcessingEngineConfig const& engine_config(panda::ChannelId const& channel_id) const;

        /**
         * @brief set the engine confugration for a specified channel
         */
        void set_engine_config(panda::ChannelId const& channel_id, panda::ProcessingEngineConfig const& config);

        /**
         * @brief set the engine configuration for the default engine
         * @details the default engine is used when there has been no specific engine_config set for a particular channel
         */
        void set_engine_config(panda::ProcessingEngineConfig const& config);

        void parse_property_tree(boost::property_tree::ptree const& pt, boost::program_options::variables_map& vm) override;

        void add_sink(std::string const& tag, std::function<panda::ConfigModule*()> const& cm);
        void add_sink(panda::ConfigModule& cm);

        /**
         * @brief return the engine allocated to the specified channel
         */
        panda::Engine& engine(panda::ChannelId const&);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        class SinkConfigs: public panda::ConfigModule
        {
            public:
                SinkConfigs();
                void add_options(OptionsDescriptionEasyInit& add_options) override;
        };

    private:
        mutable panda::DataSwitchConfig _switch_config;
        panda::ChannelsConfig _channel_config;
        SinkConfigs _sink_configs;
        mutable std::vector<DataExportStreamConfig> _sinks;
        mutable bool _exporters_init;
};

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska


#endif // SKA_CHEETAH_IO_EXPORTERS_DATAEXPORTCONFIG_H
