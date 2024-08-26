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
#include "cheetah/sigproc/WriterConfig.h"


namespace ska {
namespace cheetah {
namespace sigproc {


WriterConfig::WriterConfig(std::string const& tag)
    : utils::Config(tag)
    , _extension(".fil")
    , _max_count(0)
{
}

WriterConfig::~WriterConfig()
{
}

void WriterConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
      ("spectra_per_file", boost::program_options::value<std::size_t>(&_max_count)->default_value(_max_count), "the max number of samples per file (0=unlimited)")
      ("dir", boost::program_options::value<std::string>(&_dir), "the directory to write output files to")
      ("extension", boost::program_options::value<std::string>(&_extension)->default_value(_extension), "file extension");
}

std::string const& WriterConfig::dir() const
{
    return _dir;
}

void WriterConfig::dir(std::string const& dir)
{
    _dir = dir;
}

std::string const& WriterConfig::extension() const
{
    return _extension;
}

void WriterConfig::extension(std::string const& extension)
{
    _extension = extension;
}

std::size_t WriterConfig::max_count() const
{
    return _max_count;
}

void WriterConfig::max_count(std::size_t s)
{
    _max_count = s;
}

} // namespace sigproc
} // namespace cheetah
} // namespace ska
