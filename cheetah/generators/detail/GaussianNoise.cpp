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
#include "cheetah/generators/GaussianNoise.h"
#include "cheetah/generators/GaussianNoiseConfig.h"
#include <algorithm>

namespace ska {
namespace cheetah {
namespace generators {

template<typename DataType>
std::random_device GaussianNoise<DataType>::_dev;

template<typename DataType>
GaussianNoise<DataType>::GaussianNoise(GaussianNoiseConfig const& config)
    : BaseT()
    , _config(config)
    , _generator(_dev())
    , _rand(_config.mean(), _config.std_deviation())
{
}

template<typename DataType>
GaussianNoise<DataType>::~GaussianNoise()
{
}

template<typename DataType>
void GaussianNoise<DataType>::next(DataType& data)
{
    std::transform(data.begin(), data.end(), data.begin(), [this](typename DataType::NumericalRep) { return _rand(_generator); } );
}

} // namespace generators
} // namespace cheetah
} // namespace ska
