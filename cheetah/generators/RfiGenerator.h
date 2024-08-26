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
#ifndef SKA_CHEETAH_GENERATORS_RFIGENERATOR_H
#define SKA_CHEETAH_GENERATORS_RFIGENERATOR_H


#include "DataGenerator.h"
#include "cheetah/data/RfimFlaggedData.h"

namespace ska {
namespace cheetah {
namespace generators {

/**
 * @brief
 * @details
 */

template<typename DataType>
class RfiGenerator : public DataGenerator<DataType>
{
    protected:
        typedef DataGenerator<DataType> BaseT;
        typedef data::RfimFlaggedData<DataType> FlaggedDataType;


    public:
        RfiGenerator();
        ~RfiGenerator();

        void next(DataType& data) override;
        virtual void next(FlaggedDataType& data) = 0;

        void rfi_gaussian_block(FlaggedDataType& data
                      , std::size_t min_channel
                      , std::size_t max_channel
                      , std::size_t min_sample_number
                      , std::size_t max_sample_number
                      , float sigma);

        void rfi_ramp_block(FlaggedDataType& data
                      , std::size_t min_channel
                      , std::size_t max_channel
                      , std::size_t min_sample_number
                      , std::size_t max_sample_number);

        void rfi_broadband_block(FlaggedDataType& data
                      , std::size_t min_sample_number
                      , std::size_t max_sample_number
                      , float sigma);

};


} // namespace generators
} // namespace cheetah
} // namespace ska
#include "detail/RfiGenerator.cpp"

#endif // SKA_CHEETAH_GENERATORS_RFIGENERATOR_H
