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
namespace ska {
namespace cheetah {
namespace data {


template <typename Arch, typename T>
SpCandidate<Arch,T>::SpCandidate()
{
}


template <typename Arch, typename T>
SpCandidate<Arch,T>::SpCandidate(SpCandidate<Arch,T>::Dm dm, SpCandidate<Arch, T>::MsecTimeType tstart, SpCandidate<Arch, T>::MsecTimeType width, T sigma, std::size_t ident)
    : _dm(dm)
    , _tstart(tstart)
    , _tend(tstart)
    , _width(width)
    , _sigma(sigma)
    , _ident(ident)
{
}

template <typename Arch, typename T>
SpCandidate<Arch,T>::SpCandidate(SpCandidate<Arch,T>::Dm dm, MsecTimeType tstart, MsecTimeType width, MsecTimeType tend, T sigma, std::size_t ident)
    : _dm(dm)
    , _tstart(tstart)
    , _tend(tend)
    , _width(width)
    , _sigma(sigma)
    , _ident(ident)
{
}

template <typename Arch, typename T>
SpCandidate<Arch,T>::SpCandidate(SpCandidate<Arch,T>::Dm dm, SpCandidate<Arch, T>::MsecTimeType tstart, SpCandidate<Arch, T>::MsecTimeType width, T sigma, FrequencyType f_low, FrequencyType f_high, std::size_t ident)
    : _dm(dm)
    , _tstart(tstart)
    , _width(width)
    , _sigma(sigma)
    , _ident(ident)
{
    MsecTimeType delay = (4.15 * 1000000.0 * boost::units::si::milli * boost::units::si::seconds) * ( _dm * ( (1/(f_low * f_low)) - (1/(f_high * f_high)) )).value();
    this->_tend = _tstart + delay;
}

template <typename Arch, typename T>
SpCandidate<Arch,T>::~SpCandidate(){}


template <typename Arch, typename T>
typename SpCandidate<Arch,T>::MsecTimeType const& SpCandidate<Arch,T>::tstart() const
{
    return this->_tstart;
}

template <typename Arch, typename T>
void  SpCandidate<Arch,T>::tstart(SpCandidate<Arch,T>::MsecTimeType c_tstart)
{
    this->_tstart = c_tstart;
}

template <typename Arch, typename T>
typename SpCandidate<Arch,T>::MsecTimeType const& SpCandidate<Arch,T>::tend() const
{
    return this->_tend;
}

template <typename Arch, typename T>
void SpCandidate<Arch,T>::tend(MsecTimeType tend)
{
    this->_tend = tend;
}

template <typename Arch, typename T>
void SpCandidate<Arch,T>::duration_end(SpCandidate<Arch,T>::MsecTimeType c_tend)
{
    this->_tend = _tstart + c_tend;
}

template <typename Arch, typename T>
void  SpCandidate<Arch,T>::dm_with_duration_end(SpCandidate<Arch,T>::FrequencyType const& f_high, SpCandidate<Arch,T>::FrequencyType const& f_low, SpCandidate<Arch,T>::Dm const& dm)
{
    // set the private member for dispersion measure here
    _dm  = dm;
    //Calculate the DM smear based on the frequency
    MsecTimeType delay = (4.15 * 1000000.0 * boost::units::si::milli * boost::units::si::seconds) * ( _dm * ( (1/(f_low * f_low)) - (1/(f_high * f_high)) )).value();
    this->_tend = _tstart + delay;
}

template <typename Arch, typename T>
typename SpCandidate<Arch,T>::Dm const & SpCandidate<Arch,T>::dm() const
{
    return this->_dm;
}


template <typename Arch, typename T>
typename SpCandidate<Arch,T>::MsecTimeType const& SpCandidate<Arch,T>::width() const
{
    return this->_width;
}

template <typename Arch, typename T>
void  SpCandidate<Arch,T>::width(SpCandidate<Arch,T>::MsecTimeType c_width)
{
    this->_width = c_width;
}


template <typename Arch, typename T>
T const& SpCandidate<Arch,T>::sigma() const
{
    return this->_sigma;
}

template <typename Arch, typename T>
void  SpCandidate<Arch,T>::sigma(T c_sigma)
{
    this->_sigma = c_sigma;
}

template <typename Arch, typename T>
std::size_t const& SpCandidate<Arch,T>::ident() const
{
    return (this->_ident);
}

template <typename Arch, typename T>
void SpCandidate<Arch,T>::ident(std::size_t c_ident)
{
    this->_ident = c_ident;
}

} // namespace data
} // namespace cheetah
} // namespace ska



