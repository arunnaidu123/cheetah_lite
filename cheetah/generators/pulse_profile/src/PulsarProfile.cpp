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
#include "../PulsarProfile.h"
#include <algorithm>
#include <numeric>


namespace ska {
namespace cheetah {
namespace generators {

PulsarProfile::PulsarProfile(std::initializer_list<double> list)
    : _profile(list)
{
}

PulsarProfile::PulsarProfile()
{
}

PulsarProfile::~PulsarProfile()
{
}

PulsarProfile::Iterator PulsarProfile::begin()
{
    return _profile.begin();
}

PulsarProfile::ConstIterator PulsarProfile::cbegin() const
{
    return _profile.cbegin();
}

PulsarProfile::Iterator PulsarProfile::end()
{
    return _profile.end();
}

PulsarProfile::ConstIterator PulsarProfile::cend() const
{
    return _profile.cend();
}

void PulsarProfile::add(double value)
{
    //FrequencyType f = (_profile.size()?(_profile.back().first + _delta):_start);
    //_profile.emplace_back(std::make_pair(std::move(f), value));
    _profile.emplace_back(value);
}


PulsarProfile& operator<<(PulsarProfile& p, double value)
{
    p.add(value);
    return p;
}

double PulsarProfile::sum() const {
    return std::accumulate(cbegin(), cend(), 0.0);
                          //, [](double val, ProfileDataPoint const& spec) { return spec.second + val; });
}

std::size_t PulsarProfile::size() const
{
    return _profile.size();
}

} // namespace generators
} // namespace cheetah
} // namespace ska
