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
#ifndef SKA_CHEETAH_IO_EXPORTERS_FILESTREAMERCONFIG_H
#define SKA_CHEETAH_IO_EXPORTERS_FILESTREAMERCONFIG_H


#include "cheetah/utils/Config.h"
#include <boost/filesystem/path.hpp>

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief
 * @details
 */

class FileStreamerConfig : public utils::Config
{
    public:
        FileStreamerConfig(std::string const& tag, std::string const& default_extension);
        virtual ~FileStreamerConfig();

        /**
         * @brief get the directory specified to store files
         */
        boost::filesystem::path const& dir() const;

        /**
         * @brief set the directory in which to store files
         */
        void dir(boost::filesystem::path const&);

        /**
         * @brief get the file extension (e.g. '.cheetah')
         */
        std::string const& extension() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        boost::filesystem::path _dir;
        std::string _ext;
};


} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_IO_EXPORTERS_FILESTREAMERCONFIG_H
