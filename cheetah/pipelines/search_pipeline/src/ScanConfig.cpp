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
#include "cheetah/pipelines/search_pipeline/ScanConfig.h"


namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {


ScanConfig::ScanConfig()
    : cheetah::utils::Config("scan")
    , _freq_channels(4096)
    , _accel_range(300 *data::meters_per_second_squared)
    , _beam_bw(300 * boost::units::si::hertz*boost::units::si::mega)
    , _scan_time(600 *  boost::units::si::seconds)
    , _time_resolution(64 * boost::units::si::micro * boost::units::si::seconds)
{
}

ScanConfig::~ScanConfig()
{
}

void ScanConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("accel_range", boost::program_options::value<double>()->notifier([this](double val) {
                                        _accel_range = val * data::meters_per_second_squared;
                                     }), "Acceleration Search range")

    ("beam_bw", boost::program_options::value<double>()->notifier([this](double val) {
                                        _beam_bw = val * boost::units::si::hertz*boost::units::si::mega;
                                     }), "Beam Band width")

    ("beam_id", boost::program_options::value<int>(&_beam_id)->default_value(0), "Beam Identifier ")
    ("bit_per_sample", boost::program_options::value<int>(&_bit_per_sample)->default_value(8), "Bit per sample")
    ("disp_measure", boost::program_options::value<double>()->notifier([this](double val) {
                                        _disp_measure = val * data::parsecs_per_cube_cm;
                                    }), "Dispersion measure")

    ("freq_channels", boost::program_options::value<int>(&_freq_channels)->default_value(4096), "Number of Frequency channels")
    ("scan_id", boost::program_options::value<int>(&_scan_id)->default_value(0), "Global scan Identifier")
    ("scan_time", boost::program_options::value<int>()->notifier([this](int val) {
                                        _scan_time = val * boost::units::si::seconds;
                                    }), "Global scan time in seconds")

    ("sub_array_id", boost::program_options::value<int>(&_sub_array_id)->default_value(0), "Sub-array Identifier")
    ("time_resolution", boost::program_options::value<double>()->notifier([this](double val) {
                                    _time_resolution = val* boost::units::si::micro * boost::units::si::seconds;
                                    }), "Data sampling time (usec)")

    ("time_samples", boost::program_options::value<uint64_t>(&_time_samples)->default_value(65536), "Time samples per block")
    ("trials_number", boost::program_options::value<int>(&_trials_number)->default_value(100), "Global scan time in seconds");
}

data::AccelerationType ScanConfig::accel_range() const
{
    return _accel_range;
}

data::FrequencyType ScanConfig::beam_bw() const
{
    return _beam_bw;
}

int ScanConfig::beam_id() const
{
    return _beam_id;
}

int ScanConfig::bit_per_sample() const
{
    return _bit_per_sample;
}

typename ScanConfig::Dm ScanConfig::disp_measure() const
{
    return _disp_measure;
}

int ScanConfig::freq_channels() const
{
    return _freq_channels;
}

int ScanConfig::scan_id() const
{
    return _scan_id;
}

typename ScanConfig::ScanTimeType ScanConfig::scan_time() const
{
    return _scan_time;
}

int ScanConfig::sub_array_id() const
{
    return _sub_array_id;
}

uint64_t ScanConfig::time_samples() const
{
    return _time_samples;
}

typename ScanConfig::MicroSecTimeType ScanConfig::time_resolution() const
{
    return _time_resolution;
}

int ScanConfig::trials_number() const
{
    return _trials_number;
}


} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
