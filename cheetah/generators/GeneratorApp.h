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
#ifndef SKA_CHEETAH_GENERATORS_GENERATORAPP_H
#define SKA_CHEETAH_GENERATORS_GENERATORAPP_H

#include "cheetah/generators/DataGenerator.h"
#include "cheetah/generators/GeneratorAppConfig.h"
#include "cheetah/generators/GeneratorFactory.h"
#include "cheetah/sigproc/SigProcWriter.h"
#include <vector>
#include <memory>

namespace ska {
namespace cheetah {
namespace generators {
template<typename DataType>
class GeneratorFactory;

/**
 * @brief
 *     App to generate sigproc files using the various generators available
 */

template<typename DataType>
class GeneratorApp
{
    public:
       typedef DataGenerator<DataType> Generator;

    public:
        GeneratorApp(GeneratorAppConfig const& config, GeneratorFactory<DataType>& factory);
        ~GeneratorApp();

        void exec();

    private:
        typename std::shared_ptr<DataType> _data_ptr;
        DataType& _data;
        std::size_t _iters;
        std::vector<std::unique_ptr<Generator>> _models;                // The data generation model
        sigproc::SigProcWriter<> _writer;
};


} // namespace generators
} // namespace cheetah
} // namespace ska
#include "detail/GeneratorApp.cpp"

#endif // SKA_CHEETAH_GENERATORS_GENERATORAPP_H
