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
#include "cheetah/generators/GeneratorApp.h"
#include "pss/astrotypes/units/ModifiedJulianClock.h"


namespace ska {
namespace cheetah {
namespace generators {


template<typename DataType>
GeneratorApp<DataType>::GeneratorApp(GeneratorAppConfig const& config, GeneratorFactory<DataType>& generator_factory)
    : _data_ptr(std::make_shared<DataType>(data::DimensionSize<data::Time>(config.number_of_time_samples()), data::DimensionSize<data::Frequency>(config.number_of_channels())))
    , _data(*_data_ptr)
    , _iters(config.number_of_chunks())
    , _writer(config.sigproc_config())
{
    _data.set_channel_frequencies_const_width(config.start_frequency(), config.channel_width());
    _data.sample_interval(config.sample_interval());
    _data.start_time(pss::astrotypes::units::ModifiedJulianClock::now());
    for(auto const& generator_name : config.data_generator()) {
        _models.emplace_back(std::unique_ptr<DataGenerator<DataType>>(generator_factory.create(generator_name)));
    }
}

template<typename DataType>
GeneratorApp<DataType>::~GeneratorApp()
{
}

template<typename DataType>
void GeneratorApp<DataType>::exec()
{
    std::size_t iter = _iters;
    while(iter) {
        for(auto& model : _models) {
            model->next(_data);
        }
        _writer << _data;
        _data.start_time(_data.end_time() + _data.sample_interval());
        --iter;
    }
}

} // namespace generators
} // namespace cheetah
} // namespace ska
