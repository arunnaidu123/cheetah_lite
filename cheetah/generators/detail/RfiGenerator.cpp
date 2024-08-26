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
#include "../RfiGenerator.h"
#include <cmath>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#pragma GCC diagnostic pop

namespace ska {
namespace cheetah {
namespace generators {


template<typename DataType>
RfiGenerator<DataType>::RfiGenerator()
    : BaseT()
{
}

template<typename DataType>
RfiGenerator<DataType>::~RfiGenerator()
{
}

template<typename DataType>
void RfiGenerator<DataType>::next(DataType& data)
{
    FlaggedDataType flagged_data(data);
    this->next(flagged_data);
}

template<typename DataType>
void RfiGenerator<DataType>::rfi_gaussian_block(FlaggedDataType& flagged_data
                            , std::size_t min_channel
                            , std::size_t max_channel
                            , std::size_t min_sample_number
                            , std::size_t max_sample_number
                            , float sigma
                            )
{
    DataType& data = static_cast<DataType&>(flagged_data);
    boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::variance,boost::accumulators::tag::mean> > acc;
    for_each(data.begin(), data.end(), std::bind<void>(std::ref(acc), std::placeholders::_1));

    float mean      = boost::accumulators::mean(acc);
    float variance  = boost::accumulators::variance(acc);
    float stddev    = sqrtf(variance);

    // Choose a peak value. This should depend on the size of the data, because the highest expected stochastic outlier
    // obviously increases when you draw more samples from a normal distribution.
    // General rule is sigma>=10 for strong RFI and less for weak. That should surely be detected and will very seldom be a noise
    // peak, i.e. falsely flagged.
    float    max           = mean + sigma * stddev;

    // Determine centre of data chunk.
    int centre_sample    = int((max_sample_number + min_sample_number)/2);
    int centre_channel   = int((max_channel + min_channel)/2);
    float channel_gaussian_width = (max_channel - min_channel)/2.0;
    float time_gaussian_width = ((max_sample_number - min_sample_number)/2.0);
    float channel_gaussian_variance = powf(channel_gaussian_width/2.355, 2.0f);
    float time_gaussian_variance = powf(time_gaussian_width/2.355, 2.0f);
    auto & flags = flagged_data.rfi_flags();

    for (unsigned time_sample = min_sample_number; time_sample < max_sample_number; ++time_sample)
    {
        auto spectrum = data.spectrum(time_sample);
        auto channel_it = spectrum.begin();
        channel_it += min_channel;
        unsigned channel_number = min_channel;
        auto flag_slice = flags.spectrum(time_sample);
        auto flag_it = flag_slice.begin();
        flag_it += min_channel;
        while(channel_number <= max_channel)
        {
            *channel_it += max * std::exp(-( (powf(float(channel_number) - float(centre_channel), 2.0f)/(2.0f*channel_gaussian_variance))
                                          + (powf(float(time_sample) - float(centre_sample), 2.0f)/(2.0f*time_gaussian_variance))));
            if(time_sample<centre_sample+std::floor(time_gaussian_width)+1 &&
               time_sample>centre_sample-std::floor(time_gaussian_width)-1 &&
               channel_number<centre_channel+std::floor(channel_gaussian_width)+1 &&
               channel_number>centre_channel-std::floor(channel_gaussian_width)-1)
            {
                *flag_it = true;
            }

            ++flag_it;
            ++channel_it;
            ++channel_number;
        }
    }
}

template<typename DataType>
void RfiGenerator<DataType>::rfi_ramp_block(FlaggedDataType& flagged_data
              , std::size_t min_channel
              , std::size_t max_channel
              , std::size_t min_sample_number
              , std::size_t max_sample_number)
{
    DataType& data = static_cast<DataType&>(flagged_data);
    boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::variance,boost::accumulators::tag::mean> > acc;
    for_each(data.begin(), data.end(), std::bind<void>(std::ref(acc), std::placeholders::_1));

    float mean      = boost::accumulators::mean(acc);
    float variance  = boost::accumulators::variance(acc);
    float stddev    = sqrtf(variance);
    // Choose a peak value. This should depend on the size of the data, because the highest expected stochastic outlier
    // obviously increases when you draw more samples from a normal distribution.
    // For now, I have chosen 10 standard deviations. That should surely be detected and will very seldom be a noise
    // peak, i.e. falsely flagged.
    float    max    = mean + 10.0f * stddev;
    float    slope  = 1.0f/float(max_channel - min_channel);

    auto & flags = flagged_data.rfi_flags();

    for (unsigned time_sample = min_sample_number; time_sample < max_sample_number; ++time_sample)
    {
        auto spectrum = data.spectrum(time_sample);
        auto channel_it = spectrum.begin();
        channel_it += min_channel;
        unsigned channel_number = min_channel;
        auto flag_slice = flags.spectrum(time_sample);
        auto flag_it = flag_slice.begin();
        flag_it += min_channel;
        while(channel_number <= max_channel)
        {
            *channel_it += max * slope * (channel_number - min_channel);
            *flag_it=true;
            ++flag_it;
            ++channel_it;
            ++channel_number;
        }
    }
}

template<typename DataType>
void RfiGenerator<DataType>::rfi_broadband_block(FlaggedDataType& flagged_data
                            , std::size_t min_sample_number
                            , std::size_t max_sample_number
                            , float sigma
                            )
{
    DataType& data = static_cast<DataType&>(flagged_data);
    boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::variance,boost::accumulators::tag::mean> > acc;
    for_each(data.begin(), data.end(), std::bind<void>(std::ref(acc), std::placeholders::_1));

    float mean      = boost::accumulators::mean(acc);
    float variance  = boost::accumulators::variance(acc);
    float stddev    = sqrtf(variance);
    std::size_t min_channel = 1;
    std::size_t max_channel = flagged_data.number_of_channels()-1;

    //width of the gaussian pulse is taken as the half of the given time range. We can compute the width of the gaussian
    float width = float((max_sample_number-min_sample_number)/2.0f);
    float variance_of_gaussian =  powf(width/2.355, 2.0f);

    // Choose a peak value. This should depend on the size of the data, because the highest expected stochastic outlier
    // obviously increases when you draw more samples from a normal distribution.
    // General rule is sigma>=10 for strong RFI and less for weak. That should surely be detected and will very seldom be a noise
    // peak, i.e. falsely flagged.
    float    max           = mean + sigma * stddev;

    // Determine centre of data chunk.
    int centre_sample    = int((max_sample_number + min_sample_number)/2);

    auto & flags = flagged_data.rfi_flags();

    for (unsigned time_sample = min_sample_number; time_sample < max_sample_number; ++time_sample)
    {
        auto spectrum = data.spectrum(time_sample);
        auto channel_it = spectrum.begin();
        channel_it += min_channel;
        unsigned channel_number = min_channel;
        auto flag_slice = flags.spectrum(time_sample);
        auto flag_it = flag_slice.begin();
        flag_it += min_channel;
        while(channel_number <= max_channel)
        {
            *channel_it += max * std::exp(-(powf(float(time_sample) - float(centre_sample), 2.0f))/(2.0f*variance_of_gaussian));
            if(time_sample<centre_sample+std::floor(width)+1 &&
               time_sample>centre_sample-std::floor(width)-1)
            {
                *flag_it = true;
            }

            ++flag_it;
            ++channel_it;
            ++channel_number;
        }
    }
}

} // namespace generators
} // namespace cheetah
} // namespace ska
