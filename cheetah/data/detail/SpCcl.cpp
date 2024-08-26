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
#include "cheetah/data/SpCcl.h"
#include "cheetah/utils/ModifiedJulianClock.h"
#include "pss/astrotypes/units/Time.h"
#include <algorithm>
#include <limits>

namespace ska {
namespace cheetah {
namespace data {

static const boost::units::quantity<MilliSeconds, double> dm_const_factor = (4.15 * 1000000.0 * boost::units::si::milli * boost::units::si::seconds);

template<typename NumericalRep>
SpCcl<NumericalRep>::SpCcl()
    : _dm_range(Dm(std::numeric_limits<typename Dm::value_type>::max() * parsecs_per_cube_cm),
                Dm(std::numeric_limits<typename Dm::value_type>::min() * parsecs_per_cube_cm))
{
}

template<typename NumericalRep>
SpCcl<NumericalRep>::SpCcl(SpCcl<NumericalRep>::BlocksType const& blocks, data::DimensionIndex<data::Time> offset)
    : SpCcl(blocks, (blocks.empty()?boost::units::quantity<MilliSeconds, double>(0)
                                   :pss::astrotypes::units::duration_cast<boost::units::quantity<data::MilliSeconds,double>>((double)offset * blocks.front()->sample_interval()))
           )
{
}

template<typename NumericalRep>
SpCcl<NumericalRep>::SpCcl(SpCcl<NumericalRep>::BlocksType const& blocks, boost::units::quantity<MilliSeconds, double> offset_time)
    : _dm_range(Dm(std::numeric_limits<typename Dm::value_type>::max() * parsecs_per_cube_cm),
                Dm(std::numeric_limits<typename Dm::value_type>::min() * parsecs_per_cube_cm))
    , _blocks(blocks)
    , _offset_time(offset_time)
{
    if(!blocks.empty()) {
        auto const& first_block = _blocks.front();
        _start_time=first_block->start_time() + _offset_time;
        std::pair<typename TimeFrequencyType::FrequencyType, typename TimeFrequencyType::FrequencyType> freq = first_block->low_high_frequencies();
        _dm_delay_factor = ((1.0/(freq.first * freq.first)) - (1.0/(freq.second * freq.second)));
    }
}

template<typename NumericalRep>
SpCcl<NumericalRep>::~SpCcl()
{
}

template<typename NumericalRep>
typename SpCcl<NumericalRep>::BlocksType const& SpCcl<NumericalRep>::tf_blocks() const
{
    return _blocks;
}

template<typename NumericalRep>
std::pair<typename SpCcl<NumericalRep>::Dm, typename SpCcl<NumericalRep>::Dm> const& SpCcl<NumericalRep>::dm_range() const
{
    return _dm_range;
}

template<typename NumericalRep>
void SpCcl<NumericalRep>::dm_max_min(Dm dm)
{
    if(dm > _dm_range.second) _dm_range.second = dm;
    if(dm < _dm_range.first) _dm_range.first = dm;
}

template<typename NumericalRep>
void SpCcl<NumericalRep>::push_back_calculate_duration(SpCandidateType const& cand)
{
    dm_max_min(cand.dm());
    auto start_time = cand.tstart();
    auto it = std::find_if(BaseT::rbegin()
                       , BaseT::rend()
                       , [&](SpCandidateType const& cand)
                         {
                            return start_time >= cand.tstart();
                         });
    auto r_it = BaseT::insert(it.base(), cand);
    r_it->tend(start_time + dm_const_factor * (cand.dm() * _dm_delay_factor).value());
}

template<typename NumericalRep>
void SpCcl<NumericalRep>::push_back(SpCandidateType const& cand)
{
    dm_max_min(cand.dm());
    auto start_time = cand.tstart();
    auto it = std::find_if(BaseT::rbegin()
                       , BaseT::rend()
                       , [&](SpCandidateType const& cand)
                         {
                            return start_time >= cand.tstart();
                         });
    BaseT::insert(it.base(), cand);
}

template<typename NumericalRep>
void SpCcl<NumericalRep>::emplace_back(SpCandidateType&& cand)
{
    dm_max_min(cand.dm());
    if(BaseT::empty() || cand.tstart() >= BaseT::back().tstart()) {
        BaseT::emplace_back(std::move(cand));
    }
    else {
        auto start_time = cand.tstart();
        auto it = std::lower_bound(BaseT::begin()
                , --BaseT::end()
                , start_time
                , [&](SpCandidateType const& cand, boost::units::quantity<MilliSeconds, double> start_time)
                {
                    return start_time > cand.tstart();
                });
        BaseT::insert(it,std::move(cand));
    }
}

template<typename NumericalRep>
void SpCcl<NumericalRep>::emplace(SpCandidateType&& cand)
{
    dm_max_min(cand.dm());
    auto start_time = cand.tstart();
    auto it = std::lower_bound(BaseT::begin()
            , BaseT::end()
            , start_time
            , [&](SpCandidateType const& cand, boost::units::quantity<MilliSeconds, double> start_time)
            {
                return start_time > cand.tstart();
            });

    BaseT::insert(it, std::move(cand));
}

template<typename NumericalRep>
void SpCcl<NumericalRep>::emplace_calculate_duration(SpCandidateType&& cand)
{
    dm_max_min(cand.dm());
    auto start_time = cand.tstart();
    cand.tend(start_time + dm_const_factor * (_dm_delay_factor * cand.dm()).value());
    auto it = std::lower_bound(BaseT::begin()
            , BaseT::end()
            , start_time
            , [&](SpCandidateType const& cand, boost::units::quantity<MilliSeconds, double> start_time)
            {
                return start_time > cand.tstart();
            });
    BaseT::insert(it,std::move(cand));
}

template<typename NumericalRep>
void SpCcl<NumericalRep>::emplace(typename SpCandidateType::Dm dm,
                                   boost::units::quantity<MilliSeconds, double> start_time,
                                   boost::units::quantity<MilliSeconds, double> pulse_width,
                                   float sigma,
                                   std::size_t ident)
{
    dm_max_min(dm);
    auto it = std::lower_bound(BaseT::begin()
                          , BaseT::end()
                          , start_time
                          , [&](SpCandidateType const& cand, boost::units::quantity<MilliSeconds, double> start_time)
                          {
                              return start_time > cand.tstart();
                          });
    BaseT::insert(it,SpCandidateType(dm, start_time, pulse_width
                              ,start_time + dm_const_factor * (_dm_delay_factor * dm).value()
                              ,sigma, ident));
}

template<typename NumericalRep>
void SpCcl<NumericalRep>::emplace_back(typename SpCandidateType::Dm dm,
                                   boost::units::quantity<MilliSeconds, double> start_time,
                                   boost::units::quantity<MilliSeconds, double> pulse_width,
                                   float sigma,
                                   std::size_t ident)
{
    dm_max_min(dm);
    BaseT::emplace_back(dm, start_time, pulse_width
                              ,start_time + dm_const_factor * (_dm_delay_factor * dm).value()
                              ,sigma, ident);
}

template<typename NumericalRep>
typename SpCcl<NumericalRep>::ConstDataIterator SpCcl<NumericalRep>::data_begin(CandidateWindow const& window) const
{
    if (BaseT::begin() == BaseT::end())
    {
        return ConstDataIterator(CandidateWindow(), BaseT::begin(), BaseT::end(), _blocks, _start_time, _offset_time);
    }
    else
    {
        return ConstDataIterator(window, BaseT::begin(), BaseT::end(), _blocks, _start_time, _offset_time);
    }
}

template<typename NumericalRep>
typename SpCcl<NumericalRep>::ConstDataIterator SpCcl<NumericalRep>::data_end() const
{
    return ConstDataIterator(CandidateWindow(), BaseT::end(), BaseT::end(), _blocks, _start_time, _offset_time);
}

template<typename NumericalRep>
utils::ModifiedJulianClock::time_point const& SpCcl<NumericalRep>::start_time() const
{
    return _start_time;
}

template<typename NumericalRep>
utils::ModifiedJulianClock::time_point SpCcl<NumericalRep>::start_time(SpCandidateType const& cand) const
{
    return _start_time + pss::astrotypes::units::duration_cast<utils::julian_day>(cand.tstart());
}

template<typename NumericalRep>
boost::units::quantity<data::MilliSeconds,double> SpCcl<NumericalRep>::offset_time() const
{
    return _offset_time;
}

template<typename NumericalRep>
template<typename PredicateT>
void SpCcl<NumericalRep>::remove_if(PredicateT const& pred)
{
    Dm low = _dm_range.second;
    Dm high = _dm_range.first;
    auto it = std::remove_if(begin(), end(),
                            [&pred, &low, &high](SpCandidateType const& cand) {
                                bool v = pred(cand);
                                if(!v) {
                                    if(cand.dm() > high) high = cand.dm();
                                    if(cand.dm() < low) low = cand.dm();
                                }
                                return v;
                            });

    if (it != BaseT::end())
    {
        erase(it, BaseT::end());
    }
    _dm_range.first = low;
    _dm_range.second = high;
}

///// ------------------------------------- ConstDataIterator -----------------------------
template<typename NumericalRep>
SpCcl<NumericalRep>::ConstDataIterator::ConstDataIterator( CandidateWindow const& window
                                               , ConstCandidateIterator cand_it_begin
                                               , ConstCandidateIterator const& cand_it_end
                                               , SpCcl::BlocksType const& tf_blocks
                                               , utils::ModifiedJulianClock::time_point const& epoch
                                               , boost::units::quantity<MilliSeconds, double> const& offset
                                               )
    : _window(window)
    , _cand_it(cand_it_begin)
    , _cand_it_end(cand_it_end)
    , _tf_it(tf_blocks.begin())
    , _tf_end(tf_blocks.end())
    , _tf_start_it(tf_blocks.begin())
    , _epoch(epoch)
{
    if (_cand_it != _cand_it_end)
    {
        _span = ((*_tf_it)->number_of_spectra() * static_cast<boost::units::quantity<MilliSeconds,double>>((*_tf_it)->sample_interval()));
        _duration = _span - offset;
        this->next_candidate();
    }
}

template<typename NumericalRep>
void SpCcl<NumericalRep>::ConstDataIterator::next_candidate()
{
    if( _cand_it == _cand_it_end )
    {
        return;
    }
    auto increment_block = [this]()
    {
        ++_tf_it;
        if (_tf_it == _tf_end)
        {
            return false;
        }
        _span = ((*_tf_it)->number_of_spectra() * static_cast<boost::units::quantity<MilliSeconds,double>>((*_tf_it)->sample_interval()));
        _duration += _span;
        return true;
    };

    boost::units::quantity<MilliSeconds, double> start_time = _cand_it->tstart() - _window.ms_before();

    if ( start_time < _duration - _span)
    {
        start_time = _duration - _span;
    }


    auto end_time =  _cand_it->tend() + _window.ms_after();
    auto start_cand = _cand_it;

    // determine overlapping candidates
    while(++_cand_it != _cand_it_end)
    {
        if(_cand_it->tstart() - _window.ms_before() <= end_time) {
            auto next_end_time = _cand_it->tend() + _window.ms_after();
            if(next_end_time > end_time) end_time = next_end_time;
        }
        else {
            break;
        }
    }
    // _cand_it is now in another data block
    _cands.emplace_back(start_cand, _cand_it); // mark which candidate overlap taking into account windowing

    while(start_time >= _duration)
    {
        if (!increment_block())
        {
            throw panda::Error("Candidate Start time larger than end of data!!");
        }
    }

    // set start time of the candidate block in MJD
    _start_time = _epoch + pss::astrotypes::units::duration_cast<utils::julian_day>(start_time);
    data::DimensionIndex<data::Time> start_offset((*_tf_it)->number_of_spectra() - (std::size_t)(((_duration - start_time)/static_cast<boost::units::quantity<MilliSeconds,double>>((*_tf_it)->sample_interval()))));

    while (end_time > _duration)
    {
        data::DimensionIndex<data::Time> end_offset((std::size_t)((*_tf_it)->number_of_spectra()-1));
        _slices.push_back((*_tf_it)->slice(data::DimensionSpan<data::Time>(start_offset, end_offset)));
        start_offset = data::DimensionIndex<data::Time>(0);
        if (!increment_block())
        {
            _slices.back()._last_block = true;
            return;
        }
    }
    data::DimensionIndex<data::Time> end_offset((*_tf_it)->number_of_spectra() - 1 - (std::size_t)(((_duration - end_time)/static_cast<boost::units::quantity<MilliSeconds,double>>((*_tf_it)->sample_interval()))));
    _slices.emplace_back((*_tf_it)->slice(data::DimensionSpan<data::Time>(start_offset,end_offset)), true);

}

template<typename NumericalRep>
bool SpCcl<NumericalRep>::ConstDataIterator::end_block() const
{
    return _slices.front().last_block();
}

template<typename NumericalRep>
utils::ModifiedJulianClock::time_point const& SpCcl<NumericalRep>::ConstDataIterator::start_time() const
{
    return _start_time;
}

template<typename NumericalRep>
typename SpCcl<NumericalRep>::ConstDataIterator& SpCcl<NumericalRep>::ConstDataIterator::operator++()
{
    auto& slice = _slices.front();
    _start_time += pss::astrotypes::units::duration_cast<utils::julian_day>(slice.slice().template dimension<data::Time>() * (*_tf_start_it)->sample_interval());

    if(slice.last_block()) {
        // keep candidate info up to date
        _cands.pop_front();
    }
    _slices.pop_front();
    if(_slices.size() == 0)
    {
        next_candidate();
    }
    return *this;
}

template<typename NumericalRep>
typename SpCcl<NumericalRep>::ConstCandidateIterator SpCcl<NumericalRep>::ConstDataIterator::candidate_begin() const
{
    return _cands.front().first;
}

template<typename NumericalRep>
typename SpCcl<NumericalRep>::ConstCandidateIterator const& SpCcl<NumericalRep>::ConstDataIterator::candidate_end() const
{
    return _cands.front().second;
}

template<typename NumericalRep>
typename SpCcl<NumericalRep>::ConstDataIterator SpCcl<NumericalRep>::ConstDataIterator::operator++(int)
{
    ConstDataIterator tmp(*this);
    ++*this;
    return tmp;
}

template<typename NumericalRep>
typename SpCcl<NumericalRep>::ConstDataIterator::const_reference SpCcl<NumericalRep>::ConstDataIterator::operator*() const
{
    return _slices.front().slice();
}

template<typename NumericalRep>
bool SpCcl<NumericalRep>::ConstDataIterator::operator==(ConstDataIterator const& d) const
{
    if(d._slices.size() != this->_slices.size())
    {
        return false;
    }

    if(d._slices.size() == 0) return true;

    return d._slices.front().slice() == this->_slices.front().slice();
}

template<typename NumericalRep>
bool SpCcl<NumericalRep>::ConstDataIterator::operator!=(ConstDataIterator const& d) const
{
    return !(d == *this);
}

} // namespace data
} // namespace cheetah
} // namespace ska
