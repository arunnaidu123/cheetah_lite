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
#ifndef SKA_CHEETAH_SIGPROC_WRITERCONFIG_H
#define SKA_CHEETAH_SIGPROC_WRITERCONFIG_H


#include "cheetah/utils/Config.h"

namespace ska {
namespace cheetah {
namespace sigproc {

/**
 * @brief
 *    Configuration options for a SigProcWriter
 * @details
 * 
 */

class WriterConfig : public utils::Config
{
    public:
        WriterConfig(std::string const& tag = "sigproc");
        ~WriterConfig();

        std::string const& dir() const;
        void dir(std::string const&);
        std::string const& extension() const;
        void extension(std::string const& extension);

        std::size_t max_count() const;
        void max_count(std::size_t);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        std::string _dir;
        std::string _extension;
        std::size_t _max_count;
};


} // namespace sigproc
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_SIGPROC_WRITERCONFIG_H 
