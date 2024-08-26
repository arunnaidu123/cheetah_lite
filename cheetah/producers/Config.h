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
#ifndef SKA_CHEETAH_PRODUCERS_CONFIG_H
#define SKA_CHEETAH_PRODUCERS_CONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/data/SpCandidateData.h"
#include "cheetah/io/exporters/SpCclSpeadReaderConfig.h"

namespace ska {
namespace cheetah {
namespace producers {

/**
 * @brief Producer Configurations
 * @details Configuration are templated on the type of Data they produce
 */
template<typename DataType>
class Config;


/**
 * @brief Configurations fo data::SpCandidateData producers
 */
template<typename NumericalT>
class Config<data::SpCandidateData<NumericalT>> : public utils::Config
{
        typedef utils::Config BaseT;

    public:
        Config();
#ifdef ENABLE_SPEAD
        io::exporters::SpCclSpeadReaderConfig const& spead_config() const;
#endif // ENABLE_SPEAD

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
#ifdef ENABLE_SPEAD
        io::exporters::SpCclSpeadReaderConfig _spccl_spead_config;
#endif // ENABLE_SPEAD
};


} // namespace producers
} // namespace cheetah
} // namespace ska
#include "detail/Config.cpp"

#endif // SKA_CHEETAH_PRODUCERS_CONFIG_H
