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
#include "cheetah/sigproc/Config.h"


namespace ska {
namespace cheetah {
namespace sigproc {

static const int default_nbits=8;

Config::Config()
    : cheetah::utils::Config("sigproc")
    , _sigproc_files()
    , _nbits(default_nbits)
    , _chunk_samples(0)
{
}

Config::~Config()
{
}


void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("file", boost::program_options::value<std::vector<std::string>>(&_sigproc_files)->multitoken(), "specify the sigproc file(s) to read as input data")
    ("chunk_samples", boost::program_options::value<unsigned>(&_chunk_samples)->default_value(128), "the number of time samples in each chunk")
    ("default-nbits", boost::program_options::value<int>(&_nbits)->default_value(default_nbits), "specify the default number of bits to use when not specified in sigproc header");
}

std::vector<std::string> const& Config::sigproc_files() const
{
    return _sigproc_files;
}

void Config::set_sigproc_files(std::vector<std::string> const& sigproc_files)
{
    _sigproc_files = sigproc_files;
}

void Config::set_sigproc_files(std::string sigproc_file)
{
    _sigproc_files.emplace_back(sigproc_file);
}

int Config::nbits() const
{
    return _nbits;
}

unsigned Config::chunk_samples() const
{
    return _chunk_samples;
}

void Config::set_chunk_samples(unsigned num)
{
    _chunk_samples = num;
}

} // namespace sigproc
} // namespace cheetah
} // namespace ska
