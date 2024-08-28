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
#include "cheetah/io/exporters/DataExportConfig.h"
#include "cheetah/io/exporters/OcldFileStreamerConfig.h"
#include "cheetah/io/exporters/SclFileStreamerConfig.h"
#include "cheetah/io/exporters/SclCandidateDataStreamerConfig.h"
#include "cheetah/io/exporters/SpCclFileStreamerConfig.h"
#include "cheetah/io/exporters/SpCandidateDataStreamerConfig.h"
#ifdef ENABLE_SPEAD
#include "cheetah/io/exporters/SpCclSpeadStreamerConfig.h"
#endif // ENABLE_SPEAD
#include "cheetah/sigproc/WriterConfig.h"
#include "panda/ChannelInfo.h"
#include "panda/ChannelId.h"


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

DataExportConfig::SinkConfigs::SinkConfigs()
    : panda::ConfigModule("sink_configs")
{
}

void DataExportConfig::SinkConfigs::add_options(OptionsDescriptionEasyInit& )
{
}

DataExportConfig::DataExportConfig( std::string const& tag)
    : utils::Config(tag)
    , _exporters_init(false)
{
    number_of_threads(0); // syncronous mode by default
    add(_channel_config);
    add(_sink_configs);
    _sink_configs.add_factory("sigproc", []() { return new sigproc::WriterConfig(); });
    _sink_configs.add_factory("spccl_files", []() { return new SpCclFileStreamerConfig(); });
    _sink_configs.add_factory("sp_candidate_data", []() { return new SpCandidateDataStreamerConfig(); });
    _sink_configs.add_factory("ocld_files", []() { return new OcldFileStreamerConfig(); });
    _sink_configs.add_factory("scl_files", []() { return new SclFileStreamerConfig(); });
    _sink_configs.add_factory("scl_candidate_data", []() { return new SclCandidateDataStreamerConfig(); });
    //_sink_configs.add_factory("spccl_sigproc_files", []() { return new SpCclSigProcConfig(); });
#ifdef ENABLE_SPEAD
    _sink_configs.add_factory("spccl_spead", []() { return new SpCclSpeadStreamerConfig(); });
#endif // ENABLE_SPEAD
    _sink_configs.add_default_function([](std::string const& tag) {
                                PANDA_LOG_WARN << "unknown sink type: " << tag;
                        });
}

DataExportConfig::~DataExportConfig()
{
}

void DataExportConfig::number_of_threads(unsigned const& number_of_threads)
{
    panda::ProcessingEngineConfig config(number_of_threads);
    config.number_of_threads(number_of_threads);
    _switch_config.set_engine_config(config);
}

void DataExportConfig::activate_streams(std::vector<std::string> const& streams)
{
    for(auto const& stream : streams) {
         _switch_config.activate_channel(panda::ChannelId(stream));
    }
}

void DataExportConfig::activate(panda::ChannelId const& stream)
{
     _switch_config.activate_channel(stream);
}

void DataExportConfig::add_exporter(DataExportStreamConfig exporter_config)
{
    _sinks.emplace_back(exporter_config);
}

panda::Engine& DataExportConfig::engine(panda::ChannelId const& id)
{
    return _switch_config.engine(id);
}

std::vector<DataExportStreamConfig> const& DataExportConfig::exporters() const
{
    if( ! _exporters_init ) {
        _exporters_init = true;
        for( panda::ChannelInfo const* channel : _channel_config.channels() ) {
            if(channel->active()) {
                _switch_config.activate_channel(channel->id());
            } else {
                _switch_config.deactivate_channel(channel->id());
            }
            for( auto const& sink_id : channel->sinks() ) {
                if(sink_id != "") {
                    bool id_found=false;
                    for( auto const& section : _sink_configs.subsections() )
                    {
                        auto it=_sink_configs.subsection(section);
                        while(it != _sink_configs.subsection_end()) {
                            if(it->id() == sink_id) {
                                id_found = true;
                                _sinks.emplace_back(DataExportStreamConfig(channel->id()
                                                  , _switch_config.engine(channel->id())
                                                  , ExporterType(section)
                                                  , static_cast<utils::Config const&>(*it)));
                            }
                            ++it;
                        }
                    }
                    if(!id_found) {
                        PANDA_LOG_ERROR << "Channel error (" << channel->id().to_string()  << "): sink with id=" << sink_id << " not found";
                    }
                }
            }
        }
    }
    return _sinks;
}

void DataExportConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
        ("threads", boost::program_options::value<unsigned>()->default_value(0u)->notifier(std::bind(&DataExportConfig::number_of_threads, this, std::placeholders::_1)), "set the number of dedicated threads to run data export services");
}

panda::DataSwitchConfig& DataExportConfig::switch_config() const
{
    return _switch_config;
}

void DataExportConfig::parse_property_tree(boost::property_tree::ptree const& pt, boost::program_options::variables_map& vm)
{
    utils::Config::parse_property_tree(pt, vm); // call the base method
}

panda::ProcessingEngineConfig const& DataExportConfig::engine_config(panda::ChannelId const& channel_id) const
{
    return _switch_config.engine_config(channel_id);
}

void DataExportConfig::set_engine_config(panda::ChannelId const& channel_id, panda::ProcessingEngineConfig const& config)
{
    _switch_config.set_engine_config(channel_id, config);
}

void DataExportConfig::set_engine_config(panda::ProcessingEngineConfig const& config)
{
    _switch_config.set_engine_config(config);
}

void DataExportConfig::channel(panda::ChannelInfo const& channel)
{
    _channel_config.channel(channel);
}

void DataExportConfig::add_sink(std::string const& tag, std::function<panda::ConfigModule*()> const& cm)
{
    _sink_configs.add_factory(tag, cm);
}

void DataExportConfig::add_sink(panda::ConfigModule& cm)
{
    _sink_configs.add(cm);
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
