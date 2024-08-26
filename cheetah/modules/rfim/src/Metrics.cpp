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
#include "cheetah/modules/rfim/Metrics.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {


Metrics::Metrics(TimeFrequencyFlagsType const& expected, TimeFrequencyFlagsType const& actual)
{
    compare(expected, actual);
}

Metrics::~Metrics()
{
}

std::size_t Metrics::num_correct() const
{
    return _correct_num;
}

std::size_t Metrics::num_rfi() const
{
    return _rfi_num;
}

float Metrics::rfi_detected_percentage() const
{
    if(_rfi_num)
        return (100.0*_correct_num)/_rfi_num;
    return 100.0;
}

float Metrics::correct_percentage() const
{
    return 100.0*((double)(_false_positives.data_size() - (_false_positives_num + _false_negatives_num))/_false_positives.data_size());
}

float Metrics::false_positives_percentage() const
{
    return (100.0*_false_positives_num)/_false_positives.data_size();
}

float Metrics::false_negatives_percentage() const
{
    return (100.0*_false_negatives_num)/_false_negatives.data_size();
}

typename Metrics::TimeFrequencyFlagsType const& Metrics::false_positives() const
{
    return _false_positives;
}

std::size_t Metrics::num_false_positives() const
{
    return _false_positives_num;
}

typename Metrics::TimeFrequencyFlagsType const& Metrics::false_negatives() const
{
    return _false_negatives;
}

std::size_t Metrics::num_false_negatives() const
{
    return _false_negatives_num;
}

std::size_t Metrics::compare(TimeFrequencyFlagsType const& expected, TimeFrequencyFlagsType const& actual)
{
    _false_positives_num = 0;
    _false_negatives_num = 0;
    _correct_num = 0;
    _rfi_num = 0;

    data::DimensionSize<data::Time> max_spectra(std::max(expected.number_of_spectra(), actual.number_of_spectra()));
    data::DimensionSize<data::Frequency> max_channels(std::max(expected.number_of_channels(), actual.number_of_channels()));
    data::DimensionSize<data::Time> min_samples(std::min(expected.number_of_spectra(), actual.number_of_spectra()));
    data::DimensionSize<data::Frequency> min_channels(std::min(expected.number_of_channels(), actual.number_of_channels()));
    PANDA_LOG_DEBUG << "max_spectra=" << max_spectra << " max_channels=" << max_channels;
    if(expected.data_size() != actual.data_size())
    {
        panda::Error e("unmatched flag size: samples(");
        e << max_spectra << " vs " << min_samples;
        e << ") channels(" << max_channels << " vs " << min_channels;
    }

    _false_positives.resize(max_spectra, max_channels);
    _false_negatives.resize(max_spectra, max_channels);

    // now ananlyse the overlapping flags
    auto it = expected.begin();
    auto actual_it = actual.begin();
    auto fp_it = _false_positives.begin();
    auto fn_it = _false_negatives.begin();
    while(it != expected.end())
    {
        if(*it) ++_rfi_num;
        if(*it == *actual_it) {
            *fp_it = false;
            *fn_it = false;
            if(*it) ++_correct_num;
        }
        else{
            if(*it) {
                *fp_it = false;
                *fn_it = true;
                ++_false_negatives_num;
            }
            else {
                *fp_it = true;
                ++_false_positives_num;
                *fn_it = false;
            }
        }
        // note: probably should investigate intrusive containers to reduce overhead of maintianing multiple iterators
        ++it;
        ++actual_it;
        ++fp_it;
        ++fn_it;
    }
    return _false_positives_num + _false_negatives_num;
}

} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
