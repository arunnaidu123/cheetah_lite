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
#include <iomanip>
namespace ska {
namespace cheetah {
namespace io {
namespace exporters {


template<typename TimeFrequencyT>
SpCandidateDataStreamerTraits<TimeFrequencyT>::SpCandidateDataStreamerTraits()
{
}

template<typename TimeFrequencyT>
SpCandidateDataStreamerTraits<TimeFrequencyT>::~SpCandidateDataStreamerTraits()
{
}

template<typename TimeFrequencyT>
void SpCandidateDataStreamerTraits<TimeFrequencyT>::write(std::ostream& os, data::SpCandidateData<TimeFrequencyT> const& candidate_list)
{
    if(candidate_list.data_size()==0) return;

    for(auto it = candidate_list.begin_candidates(); it != candidate_list.end_candidates(); ++it)
    {
        auto const& candidate = *it;
        auto start_mjd = candidate.start_time();

        os << std::left << std::setw(_column_width) << std::setprecision(15) << start_mjd
           << std::left << std::setw(_column_width) << "\t" << std::setprecision(6) << candidate.dm().value()
           << std::left << std::setw(_column_width) << "\t" << std::setprecision(6) << candidate.width().value()
           << std::left << std::setw(_column_width) << "\t" << std::setprecision(6) << candidate.sigma() << "\n";
    }
    os.flush();
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
