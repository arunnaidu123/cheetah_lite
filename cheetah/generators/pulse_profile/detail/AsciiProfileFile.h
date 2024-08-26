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
#ifndef SKA_CHEETAH_GENERATORS_ASCIIPROFILEFILE_H
#define SKA_CHEETAH_GENERATORS_ASCIIPROFILEFILE_H

#include "cheetah/generators/pulse_profile/PulsarProfile.h"
#include <boost/filesystem.hpp>

namespace ska {
namespace cheetah {
namespace generators {

/**
 * @brief
 *    parse ascii files with lines of the format
 *      bin_number intesity_value
 */

class AsciiProfileFile
{
    public:
        AsciiProfileFile(boost::filesystem::path const& filename);
        ~AsciiProfileFile();
        PulsarProfile const& read() const;
        void write(PulsarProfile);

    private:
        mutable PulsarProfile _profile;
        boost::filesystem::path _filename;
};


} // namespace generators
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_GENERATORS_ASCIIPROFILEFILE_H
