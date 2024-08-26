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
#include "cheetah/io/exporters/SpCandidateDataStreamer.h"


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {


template<typename TimeFrequencyT>
SpCandidateDataStreamer<TimeFrequencyT>::SpCandidateDataStreamer(SpCandidateDataStreamerConfig const& config)
    : BaseT(config.dir())
{
    this->extension(config.extension());
}

template<typename TimeFrequencyT>
SpCandidateDataStreamer<TimeFrequencyT>::~SpCandidateDataStreamer()
{
}

template<typename TimeFrequencyT>
boost::filesystem::path SpCandidateDataStreamer<TimeFrequencyT>::next_filename(data::SpCandidateData<TimeFrequencyT> const& data)
{
    if(data.data_size() ==0 )
        return BaseT::next_filename(data);
    std::time_t ttp = std::chrono::system_clock::to_time_t(data.start_time());
    char stem[20];
    std::strftime(stem, sizeof(stem), "%Y_%m_%d_%H:%M:%S", std::gmtime(&ttp));
    boost::filesystem::path file = boost::filesystem::path(this->_prefix + stem + this->_extension);
    return file;
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
