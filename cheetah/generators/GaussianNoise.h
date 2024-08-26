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
#ifndef SKA_CHEETAH_GENERATORS_GAUSSIANNOISE_H
#define SKA_CHEETAH_GENERATORS_GAUSSIANNOISE_H


#include "DataGenerator.h"
#include <random>

namespace ska {
namespace cheetah {
namespace generators {
class GaussianNoiseConfig;

/**
 * @brief
 *
 * @details
 *
 */

template<typename DataType>
class GaussianNoise : public DataGenerator<DataType>
{
        typedef DataGenerator<DataType> BaseT;
        typedef typename DataType::NumericalRep NumericalRep;

    public:
        GaussianNoise(GaussianNoiseConfig const&);
        GaussianNoise(GaussianNoise&&) = default;
        ~GaussianNoise();

        virtual void next(DataType& data) override;

    private:
        GaussianNoiseConfig const& _config;
        static std::random_device _dev;
        std::mt19937 _generator;
        std::normal_distribution<float> _rand;
};


} // namespace generators
} // namespace cheetah
} // namespace ska
#include "detail/GaussianNoise.cpp"

#endif // SKA_CHEETAH_GENERATORS_GAUSSIANNOISE_H
