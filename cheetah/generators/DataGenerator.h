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

#ifndef SKA_CHEETAH_GENERATORS_DATAGENERATOR_H
#define SKA_CHEETAH_GENERATORS_DATAGENERATOR_H

namespace ska {
namespace cheetah {
namespace generators {

/**
 * @brief Base class for all generators
 * @tparam DataType is the type of data to be filled by thre generator
 *         It must have typedefs for:
 *            FrequencyType e.g. MegaHz
 *            TimeType      e.g. MilliSeconds
 *            TimePointType e.g. MJD
 */

template<typename DataType>
class DataGenerator
{
    public:
        typedef typename DataType::FrequencyType FrequencyType;
        typedef typename DataType::TimeType TimeType;
        typedef typename DataType::TimePointType TimePointType;

    public:
        DataGenerator();
        virtual ~DataGenerator() = 0;
        virtual void next(DataType& data) = 0;
};


} // namespace generators
} // namespace cheetah
} // namespace ska
#include "detail/DataGenerator.cpp"

#endif // SKA_CHEETAH_GENERATORS_DATAGENERATOR_H
