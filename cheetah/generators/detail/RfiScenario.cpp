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
#include "cheetah/generators/RfiScenario.h"
#include <random>


namespace ska {
namespace cheetah {
namespace generators {


// -------------------- Scenario 0 ---------------------
template<typename DataType>
RfiScenario<0, DataType>::RfiScenario()
{
}

template<typename DataType>
RfiScenario<0, DataType>::~RfiScenario()
{
}

template<typename DataType>
void RfiScenario<0, DataType>::next(FlaggedDataType&)
{
}

template<typename DataType>
constexpr char RfiScenario<0, DataType>::description[];

// -------------------- Scenario 1 ---------------------
template<typename DataType>
RfiScenario<1, DataType>::RfiScenario()
{
}

template<typename DataType>
RfiScenario<1, DataType>::~RfiScenario()
{
}

template<typename DataType>
constexpr char RfiScenario<1, DataType>::description[];

template<typename DataType>
void RfiScenario<1, DataType>::next(FlaggedDataType& data)
{
    unsigned factor = 4;
    std::size_t min_channel  = data.number_of_channels()/factor;
    std::size_t max_channel  = (factor - 1) * min_channel;
    std::size_t min_sample   = data.number_of_spectra()/factor;
    std::size_t max_sample   = (factor - 1) * min_sample;

    this->rfi_ramp_block(data, min_channel, max_channel, min_sample, max_sample);
}




// -------------------- Scenario 2 ---------------------
template<typename DataType>
RfiScenario<2, DataType>::RfiScenario()
{
}

template<typename DataType>
RfiScenario<2, DataType>::~RfiScenario()
{
}

template<typename DataType>
constexpr char RfiScenario<2, DataType>::description[];

template<typename DataType>
void RfiScenario<2, DataType>::next(FlaggedDataType& data)
{
    std::size_t pulse_channel_centre = data.number_of_channels()/2;
    std::size_t pulse_sample_centre = data.number_of_spectra()/2;
    unsigned channel_range = data.number_of_channels()/4;
    unsigned sample_range = data.number_of_spectra()/2;
    if(channel_range>32) channel_range=32;
    if(sample_range>64) sample_range=64;
    std::size_t min_channel  = pulse_channel_centre-channel_range;
    std::size_t max_channel  = pulse_channel_centre+channel_range;
    std::size_t min_sample   = pulse_sample_centre-sample_range;
    std::size_t max_sample   = pulse_sample_centre+sample_range;

    this->rfi_gaussian_block(data, min_channel, max_channel, min_sample, max_sample,6.0);
}

// -------------------- Scenario 3 ---------------------
template<typename DataType>
RfiScenario<3, DataType>::RfiScenario()
{
}

template<typename DataType>
RfiScenario<3, DataType>::~RfiScenario()
{
}

template<typename DataType>
constexpr char RfiScenario<3, DataType>::description[];

template<typename DataType>
void RfiScenario<3, DataType>::next(FlaggedDataType& data)
{
    unsigned channel_range = data.number_of_channels()/4;
    unsigned sample_range = data.number_of_spectra()/2;
    if(channel_range>32) channel_range = 32;
    if(sample_range>64) sample_range = 64;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(sample_range, data.number_of_spectra()-sample_range);

    std::size_t pulse_channel_centre = data.number_of_channels()/4;
    std::size_t min_channel  = pulse_channel_centre-channel_range;
    std::size_t max_channel  = pulse_channel_centre+channel_range;
    for(int i=0;i<4;++i)
    {
        if(i==2)
        {
            pulse_channel_centre = 3*data.number_of_channels()/4;
            min_channel  = pulse_channel_centre-channel_range;
            max_channel  = pulse_channel_centre+channel_range;
        }
        std::size_t pulse_sample_centre = dist(mt);

        std::size_t min_sample   = pulse_sample_centre-sample_range;
        std::size_t max_sample   = pulse_sample_centre+sample_range;
        this->rfi_gaussian_block(data, min_channel, max_channel, min_sample, max_sample,6.0);
    }
}

// -------------------- Scenario 4 ---------------------
template<typename DataType>
RfiScenario<4, DataType>::RfiScenario()
{
}

template<typename DataType>
RfiScenario<4, DataType>::~RfiScenario()
{
}

template<typename DataType>
constexpr char RfiScenario<4, DataType>::description[];

template<typename DataType>
void RfiScenario<4, DataType>::next(FlaggedDataType& data)
{
    unsigned channel_range = data.number_of_channels()/4;
    unsigned sample_range = data.number_of_spectra()/2;
    if(channel_range>32) channel_range = 32;
    if(sample_range>128) sample_range = 128;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> sample_dist(sample_range, data.number_of_spectra()-sample_range);
    std::uniform_int_distribution<int> channel_dist(channel_range, data.number_of_channels()-channel_range);
    std::uniform_int_distribution<int> sample_width_dist(sample_range/8, 2*sample_range);
    std::uniform_int_distribution<int> channel_width_dist(channel_range/4, 2*channel_range);
    unsigned number_of_pulses = 8;
    for(unsigned i=0;i<number_of_pulses;++i)
    {
        std::size_t pulse_channel_width =  channel_width_dist(mt);
        std::size_t pulse_sample_width =  sample_width_dist(mt);

        std::size_t pulse_channel_centre = channel_dist(mt);
        std::size_t min_channel  = pulse_channel_centre-pulse_channel_width/2;
        std::size_t max_channel  = pulse_channel_centre+pulse_channel_width/2;

        std::size_t pulse_sample_centre = sample_dist(mt);

        std::size_t min_sample   = pulse_sample_centre-pulse_sample_width/2;
        std::size_t max_sample   = pulse_sample_centre+pulse_sample_width/2;
        this->rfi_gaussian_block(data, min_channel, max_channel, min_sample, max_sample,6.0);
    }
}

// -------------------- Scenario 5 ---------------------
template<typename DataType>
RfiScenario<5, DataType>::RfiScenario()
{
}

template<typename DataType>
RfiScenario<5, DataType>::~RfiScenario()
{
}

template<typename DataType>
constexpr char RfiScenario<5, DataType>::description[];

template<typename DataType>
void RfiScenario<5, DataType>::next(FlaggedDataType& data)
{
    // choose a channel approx in the middle
    data::DimensionIndex<data::Frequency> channel_number(data.template dimension<data::Frequency>()/2);

    auto channel = data.tf_data().channel(channel_number);
    std::fill(channel.begin(), channel.end(), std::numeric_limits<typename DataType::NumericalRep>::max());
    auto flag_channel = data.rfi_flags().overlay(channel);
    std::fill(flag_channel.begin(), flag_channel.end(), true);
}

#ifdef NDEBUG
// -------------------- Scenario 6 ---------------------
template<typename T>
RfiScenario<6, T>::RfiScenario()
{
}

template<typename T>
RfiScenario<6, T>::~RfiScenario()
{
}

template<typename T>
constexpr char RfiScenario<6,T>::description[];

template<typename T>
void RfiScenario<6, T>::next(FlaggedDataType& data)
{
    unsigned min_channel = data.number_of_channels()/2 - data.number_of_channels()/16;
    unsigned max_channel = data.number_of_channels()/2 + data.number_of_channels()/16;

    unsigned factor                   =  128;
    unsigned sample_stride            =  data.number_of_spectra()/(4*factor);

    std::size_t min_sample            =  1*sample_stride;
    std::size_t max_sample            =  3*sample_stride;

    for(unsigned i=0; i<factor-1; ++i)
    {
        min_sample            +=  4*sample_stride;
        max_sample            +=  4*sample_stride;

        this->rfi_gaussian_block(data, min_channel, max_channel, min_sample, max_sample,6.0);
    }
}

// -------------------- Scenario 7 ---------------------
template<typename T>
RfiScenario<7, T>::RfiScenario()
{
}

template<typename T>
RfiScenario<7, T>::~RfiScenario()
{
}

template<typename T>
constexpr char RfiScenario<7,T>::description[];

template<typename T>
void RfiScenario<7, T>::next(FlaggedDataType& data)
{
    unsigned sample_range = data.number_of_spectra()/4;
    if(sample_range>8) sample_range = 8;

    std::size_t min_sample            =  data.number_of_spectra()/2 - sample_range;
    std::size_t max_sample            =  data.number_of_spectra()/2 + sample_range;

    this->rfi_broadband_block(data, min_sample, max_sample,6.0);
}

// -------------------- Scenario 8 ---------------------
template<typename T>
RfiScenario<8, T>::RfiScenario()
{
}

template<typename T>
RfiScenario<8, T>::~RfiScenario()
{
}

template<typename T>
constexpr char RfiScenario<8,T>::description[];

template<typename T>
void RfiScenario<8, T>::next(FlaggedDataType& data)
{
    unsigned min_channel = data.number_of_channels()/2 - data.number_of_channels()/16;
    unsigned max_channel = data.number_of_channels()/2 + data.number_of_channels()/16;

    unsigned factor                   =  128;
    unsigned sample_stride            =  data.number_of_spectra()/(4*factor);

    std::size_t min_sample            =  1*sample_stride;
    std::size_t max_sample            =  3*sample_stride;

    for(unsigned i=0; i<factor-1; ++i)
    {
        min_sample            +=  4*sample_stride;
        max_sample            +=  4*sample_stride;

        this->rfi_gaussian_block(data, min_channel, max_channel, min_sample, max_sample,4.0);
    }
}

// -------------------- Scenario 9 ---------------------
template<typename T>
RfiScenario<9, T>::RfiScenario()
{
}

template<typename T>
RfiScenario<9, T>::~RfiScenario()
{
}

template<typename T>
constexpr char RfiScenario<9,T>::description[];

template<typename T>
void RfiScenario<9, T>::next(FlaggedDataType& data)
{
    unsigned min_channel = 0;
    unsigned max_channel = data.number_of_channels()-1;

    unsigned sample_range = data.number_of_spectra()/4;
    if(sample_range>8) sample_range = 8;

    std::size_t min_sample            =  data.number_of_spectra()/2 - sample_range;
    std::size_t max_sample            =  data.number_of_spectra()/2 + sample_range;

    this->rfi_gaussian_block(data, min_channel, max_channel, min_sample, max_sample,4.0);
}

// -------------------- Scenario 10 ---------------------
template<typename T>
RfiScenario<10, T>::RfiScenario()
{
}

template<typename T>
RfiScenario<10, T>::~RfiScenario()
{
}

template<typename T>
constexpr char RfiScenario<10,T>::description[];

template<typename T>
void RfiScenario<10, T>::next(FlaggedDataType& data)
{
    unsigned factor                   =  128;
    unsigned sample_stride            =  data.number_of_spectra()/(4*factor);

    std::size_t min_sample            =  1*sample_stride;
    std::size_t max_sample            =  3*sample_stride;

    for(unsigned i=0; i<factor-1; ++i)
    {
        min_sample            +=  4*sample_stride;
        max_sample            +=  4*sample_stride;

        this->rfi_broadband_block(data, min_sample, max_sample,5.0);
    }
}
#endif // NDEBUG

} // namespace generators
} // namespace cheetah
} // namespace ska
