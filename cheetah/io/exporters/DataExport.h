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
#ifndef SKA_CHEETAH_IO_EXPORTERS_DATAEXPORT_H
#define SKA_CHEETAH_IO_EXPORTERS_DATAEXPORT_H


#include "cheetah/io/exporters/DataExportConfig.h"
#include "cheetah/io/exporters/TestProbe.h"
#include "panda/DataSwitch.h"
#include "panda/TupleUtilities.h"
#include <tuple>
#include <map>

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {
class ExporterType;
template<typename DataExporterType, typename T> class ExportInitialiser;

/**
 * @brief
 *   This class initialises a suitable panda::DataSwitch according to the
 *   exporters::DataExportConfiguration and the provided factory methods.
 *
 * @details
 *   This is a base class, and is not intended for direct use. Inherit from this class and call
 *   @ref set_factory() and @ref init() in your constructor.
 *   @ref set_factory will define the types of exporters that can be activated, and the init() will produce
 *   concrete instances of these types according to the configuration provided.
 *   @ref add_factory will allow you to define a configuration module generator to associate with the type.
 */

template<typename... DataTypes>
class DataExport : public panda::DataSwitch<DataTypes...>
{
        typedef panda::DataSwitch<DataTypes...> BaseT;

    public:
        DataExport(exporters::DataExportConfig const&);
        virtual ~DataExport() = 0; // do not use this class directly

        /**
         * @brief return the exporter types available in this class
         */
        std::set<ExporterType> const& available() const;

        /**
         * @brief activates a TestProbe object for monitoring the streamed data of DataType to a specific
         */
        template<typename DataType>
        TestProbe<DataType>& activate_test_probe(panda::ChannelId const&);

    protected:
        /**
         * @brief set a factory for generating a suitable data switch handler for the sepcified DataType and and associated type_id
         * @details This will instantiate handlers and associate them with a string identifier. The DataType must be one of DataTypes of the main class.
         *         Call this function as required in the constructor of your inheriting class before calling init()
         * @param factory. A method that produces a suitable handler given a DataExportStream configuration object.
         * @example
         * @code
         * set_factory("channel_a", [](exporters::DataExportConfig const& config) { return MyHandler(static_cast<MyHandlerConfig&>(config)); } )
         * @endcode
         */
        template<typename DataType, typename FactoryType>
        void set_factory(ExporterType const& type_id, FactoryType factory);

        /**
         * @brief initialise the output handlers using the factories supplied by set_factory(). Only call after all factories have been set
         * @details only call once, in the constructor of the derived class
         */
        void init();

    protected:
        std::tuple<ExportInitialiser<DataExport<DataTypes...>, DataTypes>...> _initialisers;
        DataExportConfig const& _config;
        std::set<ExporterType> _types;
};

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
#include "cheetah/io/exporters/detail/DataExport.cpp"

#endif // SKA_CHEETAH_IO_EXPORTERS_DATAEXPORT_H
