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
#include "cheetah/io/exporters/FileStreamerConfig.h"


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {


FileStreamerConfig::FileStreamerConfig(std::string const& tag, std::string const& default_extension)
    : utils::Config(tag)
    , _dir("output")
    , _ext(default_extension)
{
}

FileStreamerConfig::~FileStreamerConfig()
{
}

void FileStreamerConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    std::string const ext_help("the filename extension to indicate type (default: " + _ext + ")");
    std::string const dir_help("directory to write output files to (default: " + _dir.string() + ")");
    add_options
    ("extension", boost::program_options::value<std::string>(&_ext)->default_value(_ext), ext_help.c_str())
    ("dir", boost::program_options::value<boost::filesystem::path>(&_dir)->default_value(_dir), dir_help.c_str());
}

void FileStreamerConfig::dir(boost::filesystem::path const& dir)
{
    _dir = dir;
}

boost::filesystem::path const& FileStreamerConfig::dir() const
{
    return _dir;
}

std::string const& FileStreamerConfig::extension() const
{
    return _ext;
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
