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
#include "cheetah/io/exporters/SpCclSigProcConfig.h"


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {


SpCclSigProcConfig::SpCclSigProcConfig()
    : sigproc::WriterConfig("spccl_sigproc_files")
{
    add(_window_config);
    extension("cand.fil");
}

void SpCclSigProcConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    BaseT::add_options(add_options);
}

data::CandidateWindowConfig const& SpCclSigProcConfig::window_config() const
{
    return _window_config;
}

void SpCclSigProcConfig::window_config( data::CandidateWindowConfig& window_config )
{
    _window_config = window_config;
}

SpCclSigProcConfig::~SpCclSigProcConfig()
{
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
