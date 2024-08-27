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
#include "cheetah/pipelines/search_pipeline/CheetahConfig.h"
#include "cheetah/version.h"
#include "panda/ConfigFile.h"
#include "panda/Log.h"
#include "panda/PropertyTreeUtils.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

template<typename NumericalRep>
CheetahConfig<NumericalRep>::CheetahConfig(std::string const& app_name)
    : utils::Config("")
    , _desc("Generic Options")
    , _app_name(app_name)
    , _system(utils::system())
    , _stream_name("sigproc")
    , _handler_timing(false)
    , _pool_manager(_system, _pool_manager_config)
    , _ddtr_config(_pool_manager)
    , _rfim_config(_pool_manager)
    , _spdt_config(_pool_manager, _ddtr_config)
{
    // generic options
    _desc.add_options()
        //("config,c", boost::program_options::value<std::string>(), "specify the configuration file")
        ("help,h", "this help message")
        ("help-config-file", "information about the configuration file and its format")
        ("help-module", boost::program_options::value<std::vector<std::string>>(), "display help message for the specified modules (see --list-modules)")
        ("timer", "record execution time for each invocation of the computation part of the pipeline")
        ("input-stream,s", boost::program_options::value<std::string>()->default_value("sigproc")->required(), "select the input stream for the pipeline (see list-sources)")
        ("log-level", boost::program_options::value<std::vector<std::string>>()->notifier(
                    [](std::vector<std::string> const& levels) {
                        for(auto const& level : levels) {
                            if(level == "debug") {
                                panda::log_debug.enable(true);
                            }
                            else if(level == "error") {
                                panda::log_error.enable(true);
                                panda::log_warn.enable(false);
                                panda::log.enable(false);
                            }
                            else if(level == "warn") {
                                panda::log_warn.enable(true);
                                panda::log.enable(false);
                            }
                            else if(level == "log") {
                                panda::log_error.enable(true);
                                panda::log_warn.enable(true);
                                panda::log.enable(true);
                            }
                        }
                    }
                ), "set the level of logging (error, warn, log,debug)")
        ("list-sources", "list the available input streams for the pipeline and exit")
        ("list-pipelines", "list the available computational pipelines and exit")
        ("list-modules", "a list of the configurable modules (see --help-module)")
        ("pipeline,p", boost::program_options::value<std::string>(&_pipeline_name), "specify the computational pipeline to run (see list-pipelines)")
        ("config", boost::program_options::value<std::string>(), "specify a configuration file (see --help-configuration-file)")
        ("version", "print out the program version and exit")
    ;

    _options_pod.add("config", 1);

    // module options
    add(_pool_manager_config);
    add(_beam_config);
    add(_channel_mask_config);
    add(_ddtr_config);
    add(_rfim_config);
    add(_scan_config);
    add(_spdt_config);
    add(_spsift_config);
    add(_sps_clustering_config);
    add(_empty_config);
    _all_desc.add(_desc);
    _all_desc.add(command_line_options());

    // setup switch (we do not allow these to be user selected)
    panda::ProcessingEngineConfig engine_config;
    engine_config.number_of_threads(2);
    _switch_config.set_engine_config(engine_config);
}

template<typename NumericalRep>
CheetahConfig<NumericalRep>::~CheetahConfig()
{
}

template<typename NumericalRep>
void CheetahConfig<NumericalRep>::add_options(OptionsDescriptionEasyInit& )
{
}

template<typename NumericalRep>
std::string CheetahConfig<NumericalRep>::version() const
{
    return cheetah::version;
}

template<typename NumericalRep>
std::string const& CheetahConfig<NumericalRep>::stream_name() const
{
    return _stream_name;
}

template<typename NumericalRep>
std::string const& CheetahConfig<NumericalRep>::pipeline_name() const
{
    return _pipeline_name;
}

template<typename NumericalRep>
bool CheetahConfig<NumericalRep>::time_handler_invocation() const
{
    return _handler_timing;
}

template<typename NumericalRep>
void CheetahConfig<NumericalRep>::set_pipeline_handlers(std::vector<std::string> const& handler_names)
{
    _pipeline_handler_names = handler_names;
}

template<typename NumericalRep>
int CheetahConfig<NumericalRep>::parse(int argc, char** argv)
{
    if(argc == 0) return 0;
    _app_name= boost::filesystem::basename(argv[0]);

    // parse the command line
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv)
                                  .options(_all_desc)
                                  .positional(_options_pod)
                                  .run(), vm);

    if( vm.count("version") ) {
        std::cout << version() << "\n";
        return 1;
    }
    if( vm.count("help") ) {
        std::cout << "Name\n\t" << _app_name << " - cheetah pipeline launcher\n\n";
        std::cout << "Synopsis\n\t" << _app_name << " [OPTIONS] [config_file]\n\n";
        std::cout << "\tA source and a pipeline must be specfied as a minimum, either through the" << "\n";
        std::cout << "\toptions or through the config file (--help-config-file).\n" << "\n";
        std::cout << "\tModule specific help can be accessed via the --help-module flag.\n" << "\n";
        std::cout << _desc << "\n";
        return 1;
    }
    if( vm.count("help-config-file") ) {
        std::cout << "Name\n\t" << _app_name << " - cheetah pipeline launcher config files\n\n";
        std::cout << "Description\n\tconfig files should be in XML format and the filename should have the .xml extension." << "\n";
        std::cout << "\tAny command line option can be set in the config file using the long name of the option as a tag." << "\n";
        std::cout << "\tAll tags must be enclosed in the <cheetah> tag scope.\n" << "\n";
        boost::property_tree::ptree cheetah_config_pt;
        cheetah_config_pt.put_child("cheetah", config_tree());
#if BOOST_VERSION < 105600
        boost::property_tree::write_xml(std::cout, cheetah_config_pt,boost::property_tree::xml_writer_make_settings(' ', 2));
#else
        boost::property_tree::write_xml(std::cout, cheetah_config_pt,boost::property_tree::xml_writer_make_settings<std::string>(' ', 2));
#endif
        std::cout << "\n";
        return 1;
    }
    if( vm.count("list-modules") ) {
        for( auto const& mod : subsections() ) {
            std::cout << mod << "\n";
        }
        return 1;
    }
    if( vm.count("list-sources") ) {
        // list the streams
        std::vector<std::string> s = io::producers::rcpt::SkaSelector::available();
        s.push_back("sigproc");
#ifdef ENABLE_PSRDADA
            s.push_back("psrdada");
#endif // ENABLE_PSRDADA
        for( auto stream : s ) {
            std::cout << stream << "\n";
        }
        return 1;
    }
    if( vm.count("list-pipelines") ) {
        // list the pipelines
        for( auto pipeline : _pipeline_handler_names ) {
            std::cout << pipeline << "\n";
        }
        return 1;
    }
    if( vm.count("config") ) {
        // configuration file specified
        panda::ConfigFile config_file(vm["config"].as<std::string>());
        try {
            config_file.parse();
        } catch(std::exception const& e) {
            PANDA_LOG_ERROR << "exception parsing config file: " << vm["config"].as<std::string>() << " " << e.what();
            throw;
        } catch(...) {
            PANDA_LOG_ERROR << "exception parsing config file: " << vm["config"].as<std::string>();
            throw;
        }
        // <cheetah> section corresponds to generic options
        auto top_it = config_file.property_tree().find("cheetah");
        if(top_it != config_file.property_tree().not_found() ) {
            boost::program_options::store(panda::parse_property_tree<char>(top_it->second, _desc), vm);
            parse_property_tree(top_it->second, vm);
        }
    }
    // n/b must come after config file to pick up dynamic options
    if( vm.count("help-module") ) {
        for(auto const& module : vm["help-module"].as<std::vector<std::string>>() ) {
            std::cout << "Name\n\t" << _app_name << " - cheetah pipeline launcher module : " << module << "\n\n";
            std::cout << help(module) << "\n";
        }
        return 1;
    }
    if( vm.count("input-stream") ) {
        _stream_name = vm["input-stream"].as<std::string>();
    }
    if( vm.count("timer") ) {
        _handler_timing = true;
    }

    boost::program_options::notify(vm);

    return 0;
}

template<typename NumericalRep>
MultiBeamConfig<NumericalRep> const& CheetahConfig<NumericalRep>::beams_config() const
{
    return _beam_config;
}

template<typename NumericalRep>
channel_mask::ConfigurableChannelMaskConfig<NumericalRep> const& CheetahConfig<NumericalRep>::channel_mask_config() const
{
    return _channel_mask_config;
}

template<typename NumericalRep>
channel_mask::ConfigurableChannelMaskConfig<NumericalRep>& CheetahConfig<NumericalRep>::channel_mask_config()
{
    return _channel_mask_config;
}
/*
template<typename NumericalRep>
modules::ddtr::ConfigType const& CheetahConfig<NumericalRep>::ddtr_config() const
{
    return _ddtr_config;
}

template<typename NumericalRep>
modules::ddtr::ConfigType& CheetahConfig<NumericalRep>::ddtr_config()
{
    return _ddtr_config;
}
*/

template<typename NumericalRep>
modules::rfim::ConfigType<typename CheetahConfig<NumericalRep>::PoolManagerType> const& CheetahConfig<NumericalRep>::rfim_config() const
{
    return _rfim_config;
}

template<typename NumericalRep>
modules::spsift::Config const& CheetahConfig<NumericalRep>::spsift_config() const
{
    return _spsift_config;
}

template<typename NumericalRep>
modules::sps_clustering::Config const& CheetahConfig<NumericalRep>::sps_clustering_config() const
{
    return _sps_clustering_config;
}

template<typename NumericalRep>
modules::ddtr::ConfigType<typename CheetahConfig<NumericalRep>::PoolManagerType> const& CheetahConfig<NumericalRep>::ddtr_config() const
{
    return _ddtr_config;
}

template<typename NumericalRep>
modules::ddtr::ConfigType<typename CheetahConfig<NumericalRep>::PoolManagerType>& CheetahConfig<NumericalRep>::ddtr_config()
{
    return _ddtr_config;
}

template<typename NumericalRep>
modules::spdt::ConfigType<typename CheetahConfig<NumericalRep>::PoolManagerType> const& CheetahConfig<NumericalRep>::spdt_config() const
{
    return _spdt_config;
}

template<typename NumericalRep>
modules::spdt::ConfigType<typename CheetahConfig<NumericalRep>::PoolManagerType>& CheetahConfig<NumericalRep>::spdt_config()
{
    return _spdt_config;
}

template<typename NumericalRep>
panda::DataSwitchConfig& CheetahConfig<NumericalRep>::switch_config() const
{
    return _switch_config;
}

template<typename NumericalRep>
panda::PoolManagerConfig<utils::Config::SystemType> const& CheetahConfig<NumericalRep>::pool_manager_config() const
{
    return _pool_manager_config;
}

template<typename NumericalRep>
typename CheetahConfig<NumericalRep>::PoolManagerType const& CheetahConfig<NumericalRep>::pool_manager() const
{
    return _pool_manager;
}

template<typename NumericalRep>
typename CheetahConfig<NumericalRep>::PoolManagerType& CheetahConfig<NumericalRep>::pool_manager()
{
    return _pool_manager;
}

template<typename NumericalRep>
EmptyConfig const& CheetahConfig<NumericalRep>::empty_config() const
{
    return _empty_config;
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
