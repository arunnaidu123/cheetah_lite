/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
namespace io {
namespace exporters {

template<typename NumericalT>
OcldFileStreamerTraits<NumericalT>::OcldFileStreamerTraits()
{
}

template<typename NumericalT>
OcldFileStreamerTraits<NumericalT>::~OcldFileStreamerTraits()
{
}

template<typename NumericalT>
bool OcldFileStreamerTraits<NumericalT>::consistent_with_existing_file(data::Ocld<NumericalT> const&)
{
    return true;
}

template<typename NumericalT>
void OcldFileStreamerTraits<NumericalT>::headers(std::ostream& os, data::Ocld<NumericalT> const&)
{
    os << "p(" << typename data::Ocld<NumericalT>::BaseT::ValueType::CandidateType::TimeType::unit_type()
       << ")\tp_dot("
       << typename data::Ocld<NumericalT>::BaseT::ValueType::CandidateType::SecPerSecType::unit_type()
       << ")\tdm\t"
       << "width(" << typename data::Ocld<NumericalT>::BaseT::ValueType::CandidateType::TimeType::unit_type()
       << ")\tsigma\n";
}

template<typename NumericalT>
void OcldFileStreamerTraits<NumericalT>::write(std::ostream& os, data::Ocld<NumericalT> const& data)
{
    for(typename data::Ocld<NumericalT>::BaseT::ValueType const& cand : data) {
        os << cand.metadata().period()
           << "\t" << cand.metadata().pdot() << "\t"
           << cand.metadata().dm()
           << "\t" << cand.metadata().width() << "\t"
           << cand.metadata().sigma() << "\n";
    }
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
