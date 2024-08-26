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
#include "cheetah/io/exporters/DataExportStreamConfig.h"


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

DataExportStreamConfig::DataExportStreamConfig(panda::ChannelId channel_name
                                             , panda::Engine& engine
                                             , ExporterType exporter_type
                                             , utils::Config const& config)
    : _name(std::move(channel_name))
    , _engine(engine)
    , _type(std::move(exporter_type))
    , _config(config)
{
}

DataExportStreamConfig::~DataExportStreamConfig()
{
}

ExporterType const& DataExportStreamConfig::type() const
{
    return _type;
}

panda::ChannelId const& DataExportStreamConfig::channel_name() const
{
    return _name;
}

utils::Config const& DataExportStreamConfig::sink_config() const
{
    return _config;
}

panda::Engine& DataExportStreamConfig::engine() const
{
    return _engine;
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
