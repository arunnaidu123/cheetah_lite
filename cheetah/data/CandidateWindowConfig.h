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
#ifndef SKA_CHEETAH_DATA_CANDIDATEWINDOWCONFIG_H
#define SKA_CHEETAH_DATA_CANDIDATEWINDOWCONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/data/CandidateWindow.h"


namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief Config Module for initialising a CandidateWindow object
 */
class CandidateWindowConfig : public utils::Config
{
        typedef utils::Config BaseT;

    public:
        CandidateWindowConfig();
        ~CandidateWindowConfig();

        /**
         * @brief the confugred CandidateWindow
         */
        CandidateWindow const& window() const;
        CandidateWindow& window();

        /**
         * @brief set the CandidateWindow
         */
        void window(CandidateWindow& window);

        /**
         * @brief assignment operator. This will copy just the CandidateWindow from one object to another
         */
        CandidateWindowConfig& operator=( const CandidateWindowConfig& window_config );


    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        CandidateWindow _window;
};


} // namespace data
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_DATA_CANDIDATEWINDOWCONFIG_H
