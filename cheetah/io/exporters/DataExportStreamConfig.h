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
#ifndef SKA_CHEETAH_IO_EXPORTERS_DATAEXPORTSTREAMCONFIG_H
#define SKA_CHEETAH_IO_EXPORTERS_DATAEXPORTSTREAMCONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/io/exporters/ExporterType.h"
#include "panda/ChannelId.h"

namespace ska {
namespace cheetah {
namespace utils {
    class Config;
}
namespace io {
namespace exporters {

/**
 * @brief Defines the mapping of a sinks configuration block with the sink type, and the channel to associate it with
 */
class DataExportStreamConfig
{
    public:
        /**
         * @brief defines an exporter object of type @param exporter_type to be assoicated with the @channel_name
         */
        DataExportStreamConfig(panda::ChannelId channel_name, panda::Engine& engine, ExporterType exporter_type, utils::Config const& config);
        ~DataExportStreamConfig();

        /**
         * @brief return the type of exporter to associate with
         */
        ExporterType const& type() const;

        /**
         * @brief return the channel name to associate with
         */
        panda::ChannelId const& channel_name() const;

        /**
         * @brief return the sink specific configuration
         *        dynamic_cast to the actual type required
         */
        utils::Config const& sink_config() const;

        /**
         * @brief the thread-pool engine assigned to this exporter
         */
        panda::Engine& engine() const;

    private:
        panda::ChannelId _name;
        panda::Engine& _engine;
        ExporterType _type;
        utils::Config const& _config;
};

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_IO_EXPORTERS_DATAEXPORTSTREAMCONFIG_H
