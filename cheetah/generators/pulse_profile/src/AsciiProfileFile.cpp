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
#include "cheetah/generators/pulse_profile/detail/AsciiProfileFile.h"
#include <panda/Error.h>
#include <boost/filesystem/fstream.hpp>
#include <sstream>


namespace ska {
namespace cheetah {
namespace generators {


AsciiProfileFile::AsciiProfileFile(boost::filesystem::path const& filename)
    : _filename(filename)
{
}

AsciiProfileFile::~AsciiProfileFile()
{
}

void AsciiProfileFile::write(PulsarProfile profile) 
{
    _profile=std::move(profile);
    // TODO write to file
    throw panda::Error("not yet implemented");
}

PulsarProfile const& AsciiProfileFile::read() const
{
    if(boost::filesystem::exists(_filename)){
        // open the file for reading
        boost::filesystem::ifstream is(_filename);
        if(!is.is_open()) {
            panda::Error e("unable to open file: ");
            e << _filename;
            throw e;
        }
        // parse the file
        std::string line;
        double intensity;
        while( std::getline(is, line) ) {
            std::istringstream iss(line);
            iss >> intensity;
            _profile.add(intensity);
        }
    }
    return _profile;
}

} // namespace generators
} // namespace cheetah
} // namespace ska
