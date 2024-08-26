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
#include "cheetah/io/exporters/DataExport.h"
#include "cheetah/io/exporters/ExporterType.h"
#include <panda/Log.h>
#include <type_traits>


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/*
 * helper class to initialise the DataExporter by type
 */
template<typename DataExporterType, typename T>
class ExportInitialiser {

    public:
        typedef std::function<void(T&)> HandlerType;
        typedef std::function<std::function<void(T&)>(DataExportStreamConfig const&)> FactoryType;

    public:
        ExportInitialiser(DataExporterType& object)
            : _obj(object)
        {
        }

        void init_exporter(DataExportStreamConfig const& config)
        {
            auto it = _type_lookup.find(config.type());
            if( it == _type_lookup.end() ) {
                // this initialiser has no sinks associated with the type
                return; // ignore unregistered channels
            }
            // ensure we only have one object per id
            std::string const& id = config.sink_config().id();
            auto id_it = _id_lookup.find(id);
            if( id_it == _id_lookup.end() ) {
                PANDA_LOG_DEBUG << "Creating sink of type " << config.type().to_string() << " (id=" << id << ")";
                _id_lookup.insert(std::pair<std::string, HandlerType*>(id, &_obj.template add<T>(config.channel_name(), std::move((it->second).operator()(config)) )));
            } else {
                // redirect to the object already instantiated with that id
                _obj.template add<T>(config.channel_name(), *(*id_it).second );
            }
        }

        void add_factory(ExporterType const& type, FactoryType factory) {
            _type_lookup.emplace(type, std::move(factory));
        }

    private:
        DataExporterType& _obj;
        std::map<ExporterType, FactoryType> _type_lookup;
        std::map<std::string, HandlerType*> _id_lookup;
};

template<typename... DataTypes>
DataExport<DataTypes...>::DataExport(exporters::DataExportConfig const& config)
    : DataExport<DataTypes...>::BaseT(config.switch_config())
    //, _initialisers(std::make_tuple<ExportInitialiser<typename std::remove_reference<decltype(*this)>::type, DataTypes>...>(*this)...)
    , _initialisers(ExportInitialiser<typename std::remove_reference<decltype(*this)>::type, DataTypes>(*this)...)
    , _config(config)
{
}

template<typename... DataTypes>
DataExport<DataTypes...>::~DataExport()
{
}

template<typename... DataTypes>
template<typename DataType, typename FactoryType>
void DataExport<DataTypes...>::set_factory(ExporterType const& type_id, FactoryType factory)
{
    if(_types.find(type_id) == _types.end()) _types.insert(type_id);
    typedef std::tuple<ExportInitialiser<DataExport<DataTypes...>, typename std::remove_const<DataTypes>::type>...> ConstRemovedTupleType;
    //std::get<panda::Index<ExportInitialiser<DataExport<DataTypes...>, DataType>, decltype(_initialisers)>::value>(_initialisers).add_factory(type_id, std::move(factory));
    std::get<panda::Index<ExportInitialiser<DataExport<DataTypes...>, typename std::remove_const<DataType>::type>, ConstRemovedTupleType>::value>(_initialisers).add_factory(type_id, std::move(factory));
}

// helper class for panda::for_each
template<typename ConfigType >
struct LaunchInitialiser {
    public:
        LaunchInitialiser(ConfigType const& config) : _config(config) {}

        template<typename T>
        void operator()(T& initialiser) {
            initialiser.init_exporter(_config);
        }

    private:
        ConfigType const& _config;
};

template<typename... DataTypes>
void DataExport<DataTypes...>::init()
{
    for(auto const &exporter : _config.exporters()) {
        panda::for_each(_initialisers, LaunchInitialiser<DataExportStreamConfig>(exporter));
    }
}

template<typename... DataTypes>
std::set<ExporterType> const& DataExport<DataTypes...>::available() const
{
    return _types;
}

template<typename... DataTypes>
template<typename DataType>
TestProbe<DataType>& DataExport<DataTypes...>::activate_test_probe(panda::ChannelId const& id)
{
    auto& handler = this->template add<DataType>(id, TestProbe<DataType>(id));
    this->template activate<DataType>(id);

    TestProbe<DataType>* rv = handler.template target<TestProbe<DataType>>();
    assert(rv != nullptr);
    return *rv;
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
