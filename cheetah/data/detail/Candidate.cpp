/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 The SKA organisation
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


template<typename Arch, typename T>
Candidate<Arch, T>::Candidate()
    : _pulse_period(0.0 * boost::units::si::seconds)
    , _pulse_pdot(0.0)
    , _dm(0.0 * parsecs_per_cube_cm)
    , _harm_num(0)
    , _pulse_width(0.0 * milliseconds)
    , _sigma(0.0)
{
}

template<typename Arch, typename T>
Candidate<Arch, T>::Candidate(Candidate<Arch, T>::MsecTimeType period
                              , SecPerSecType pdot
                              , Candidate<Arch, T>::Dm dm
                              , std::size_t ident
                             )
    : _pulse_period(period)
    , _pulse_pdot(pdot)
    , _dm(dm)
    , _harm_num(0)
    , _pulse_width(0.0 * milliseconds)
    , _sigma(0.0)
    , _ident(ident)
{
}


template<typename Arch, typename T>
Candidate<Arch, T>::Candidate(Candidate<Arch, T>::MsecTimeType period
                              , SecPerSecType pdot
                              , Candidate<Arch, T>::Dm dm
                              , std::size_t harm_num
                              , Candidate<Arch, T>::MsecTimeType width
                              , T sigma
                              , std::size_t ident
                             )
    : _pulse_period(period)
    , _pulse_pdot(pdot)
    , _dm(dm)
    , _harm_num(harm_num)
    , _pulse_width(width)
    , _sigma(sigma)
    , _ident(ident)
{
}


template<typename Arch, typename T>
Candidate<Arch, T>::~Candidate()
{
}


template<typename Arch, typename T>
typename Candidate<Arch, T>::MsecTimeType const& Candidate<Arch,T>::period() const
{
    return _pulse_period;
}

template<typename Arch, typename T>
void Candidate<Arch, T>::period(Candidate<Arch, T>::MsecTimeType c_pulse_period)
{
    _pulse_period = c_pulse_period;
}

template<typename Arch, typename T>
typename Candidate<Arch, T>::SecPerSecType const& Candidate<Arch, T>::pdot() const
{
    return _pulse_pdot;
}

template<typename Arch, typename T>
void Candidate<Arch, T>::pdot(SecPerSecType c_pulse_pdot)
{
    _pulse_pdot = c_pulse_pdot;
}

template<typename Arch, typename T>
typename Candidate<Arch, T>::Dm const & Candidate<Arch, T>::dm() const
{
    return _dm;
}

template<typename Arch, typename T>
void Candidate<Arch, T>::dm(Dm c_dm)
{
    _dm = c_dm;
}

template<typename Arch, typename T>
std::size_t const& Candidate<Arch, T>::harmonic() const
{
    return _harm_num;
}

template<typename Arch, typename T>
void Candidate<Arch, T>::harmonic(std::size_t c_harm_num)
{
    _harm_num = c_harm_num;
}

template<typename Arch, typename T>
typename Candidate<Arch, T>::MsecTimeType const& Candidate<Arch, T>::width() const
{
    return _pulse_width;
}

template<typename Arch, typename T>
void Candidate<Arch, T>::width(Candidate<Arch, T>::MsecTimeType c_pulse_width)
{
    _pulse_width = c_pulse_width;
}

template<typename Arch, typename T>
T const& Candidate<Arch, T>::sigma() const
{
    return _sigma;
}

template<typename Arch, typename T>
void Candidate<Arch, T>::sigma(T c_sigma)
{
    _sigma = c_sigma;
}

template<typename Arch, typename T>
std::size_t const& Candidate<Arch, T>::ident() const
{
    return _ident;
}

template<typename Arch, typename T>
void Candidate<Arch, T>::ident(std::size_t c_ident)
{
    _ident = c_ident;
}

} // namespace data
} // namespace cheetah
} // namespace ska
