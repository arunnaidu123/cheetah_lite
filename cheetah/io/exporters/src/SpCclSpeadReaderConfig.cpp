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

#include "cheetah/io/exporters/SpCclSpeadReaderConfig.h"


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {


SpCclSpeadReaderConfig::SpCclSpeadReaderConfig()
    : BaseT("SpeadReader")
    , _endpoint_config("listen")
    , _init(false)
{
    _endpoint_config.address(ska::panda::IpAddress(9027, "127.0.0.1"));
    add(_endpoint_config);
    add(_engine_config);
}

SpCclSpeadReaderConfig::~SpCclSpeadReaderConfig()
{
}

void SpCclSpeadReaderConfig::add_options(OptionsDescriptionEasyInit&)
{
}

panda::IpAddress const& SpCclSpeadReaderConfig::listen_address() const
{
    return _endpoint_config.address();
}

void SpCclSpeadReaderConfig::endpoint(EndPointType const& endpoint)
{
    _endpoint = endpoint;
    _init = true;
}

typename SpCclSpeadReaderConfig::EndPointType const& SpCclSpeadReaderConfig::endpoint() const
{
    if(!_init) {
        _endpoint = _endpoint_config.address().end_point<EndPointType>();
    }
    return _endpoint;
}

SpCclSpeadReaderConfig::Engine& SpCclSpeadReaderConfig::engine() const
{
    if(!_engine) {
        _engine.reset(new Engine(_engine_config));
    }
    return *_engine;
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
