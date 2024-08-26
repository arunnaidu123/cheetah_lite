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
#include "cheetah/data/SpCandidateData.h"


namespace ska {
namespace cheetah {
namespace data {

template<typename TimeFrequencyT>
SpCandidateData<TimeFrequencyT>::SpCandidateData()
    : BaseT(std::shared_ptr<TimeFrequencyT>(nullptr))
{
}


template<typename TimeFrequencyT>
SpCandidateData<TimeFrequencyT>::SpCandidateData(std::shared_ptr<TimeFrequencyT> const& data)
    : BaseT(data)
{
}

template<typename TimeFrequencyT>
SpCandidateData<TimeFrequencyT>::~SpCandidateData()
{
}

template<typename TimeFrequencyT>
void SpCandidateData<TimeFrequencyT>::add(CandidateType&& candidate)
{
    _cands.emplace_back(std::move(candidate));
}

template<typename TimeFrequencyT>
typename SpCandidateData<TimeFrequencyT>::ConstCandidateIterator SpCandidateData<TimeFrequencyT>::begin_candidates() const
{
    return _cands.begin();
}

template<typename TimeFrequencyT>
typename SpCandidateData<TimeFrequencyT>::ConstCandidateIterator SpCandidateData<TimeFrequencyT>::end_candidates() const
{
    return _cands.end();
}

template<typename TimeFrequencyT>
std::size_t SpCandidateData<TimeFrequencyT>::number_of_candidates() const
{
    return _cands.size();
}

template<typename TimeFrequencyT>
typename SpCandidateData<TimeFrequencyT>::CandidateType const& SpCandidateData<TimeFrequencyT>::candidate(std::size_t candidate_number) const
{
    return _cands[candidate_number];
}

// ----------------- Cadidate<T> -------------------------
template<typename TimeFrequencyT>
template<typename T>
SpCandidateData<TimeFrequencyT>::Candidate<T>::Candidate()
{
}

template<typename TimeFrequencyT>
template<typename T>
SpCandidateData<TimeFrequencyT>::Candidate<T>::Candidate(Dm dm, TimePointType start_time_mjd, Width width, Duration duration, T sigma)
    : _dm(dm)
    , _start_time(start_time_mjd)
    , _duration(duration)
    , _width(width)
    , _sigma(sigma)
{
}

template<typename TimeFrequencyT>
template<typename T>
typename SpCandidateData<TimeFrequencyT>::template Candidate<T>::Dm const& SpCandidateData<TimeFrequencyT>::Candidate<T>::dm() const
{
    return _dm;
}

template<typename TimeFrequencyT>
template<typename T>
typename SpCandidateData<TimeFrequencyT>::template Candidate<T>::TimePointType const& SpCandidateData<TimeFrequencyT>::Candidate<T>::start_time() const
{
    return _start_time;
}

template<typename TimeFrequencyT>
template<typename T>
typename SpCandidateData<TimeFrequencyT>::template Candidate<T>::Duration const& SpCandidateData<TimeFrequencyT>::Candidate<T>::duration() const
{
    return _duration;
}

template<typename TimeFrequencyT>
template<typename T>
typename SpCandidateData<TimeFrequencyT>::template Candidate<T>::Sigma const& SpCandidateData<TimeFrequencyT>::Candidate<T>::sigma() const
{
    return _sigma;
}

template<typename TimeFrequencyT>
template<typename T>
typename SpCandidateData<TimeFrequencyT>::template Candidate<T>::Width const& SpCandidateData<TimeFrequencyT>::Candidate<T>::width() const
{
    return _width;
}


} // namespace data
} // namespace cheetah
} // namespace ska
