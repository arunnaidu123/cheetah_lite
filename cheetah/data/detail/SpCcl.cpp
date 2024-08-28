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
SpCcl<NumericalRep>::SpCcl(std::shared_ptr<DmTrialsType> const& data)
    : _dm_range(std::make_pair((*data->begin()).dm(),(*data->end()).dm()))
    , _start_time(data->start_time())
    , _offset_time(0.0*data::milliseconds)
{
}

template<typename NumericalRep>
SpCcl<NumericalRep>::~SpCcl()
{
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
void SpCcl<NumericalRep>::push_back(SpCandidateType const& cand)
{
    _data.push_back(cand);
}

template<typename NumericalRep>
void SpCcl<NumericalRep>::emplace_back(SpCandidateType&& cand)
{
    dm_max_min(cand.dm());
    _data.emplace_back(cand);
}

template<typename NumericalRep>
void SpCcl<NumericalRep>::emplace(SpCandidateType&& cand)
{
    dm_max_min(cand.dm());
    _data.emplace_back(cand);
}

template<typename NumericalRep>
void SpCcl<NumericalRep>::emplace(typename SpCandidateType::Dm dm,
                                   boost::units::quantity<MilliSeconds, double> start_time,
                                   boost::units::quantity<MilliSeconds, double> pulse_width,
                                   float sigma,
                                   std::size_t ident)
{
    dm_max_min(dm);
    _data.emplace_back(SpCandidateType(dm, start_time, pulse_width
                              ,start_time
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
    _data.emplace_back(SpCandidateType(dm, start_time, pulse_width
                              ,start_time
                              ,sigma, ident));
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

} // namespace data
} // namespace cheetah
} // namespace ska
