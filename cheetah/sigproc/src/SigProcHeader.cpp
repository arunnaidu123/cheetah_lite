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
#include "cheetah/sigproc/SigProcHeader.h"
#include "cheetah/data/Units.h"
#include "panda/Error.h"
#include <boost/units/quantity.hpp>

#include <cstring>
#include <string>
#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>

namespace ska {
namespace cheetah {
namespace sigproc {


// buffer for reading in data to pass to a std::string - TODO use stream buff directly
//static char _buffer[80];

// strong typing for the Header labels
/*
struct SigProcLabel : public std::string
{
    SigProcLabel() {}
    SigProcLabel(std::string const& s) : std::string(s) {}
    SigProcLabel(const char * s) : std::string(s) {}
    std::size_t size() const {
        return std::string::size() + sizeof(int32_t);
    }
};

// sigproc format types are preceded by the size
// read
template<typename T>
unsigned get_var(std::istream& stream, T& var) {
    stream.read(reinterpret_cast<char*>(&var), sizeof(T));
    return stream.gcount();
}

template<>
unsigned get_var<SigProcHeader::FrequencyType>(std::istream& stream, SigProcHeader::FrequencyType& var) {
    typedef SigProcHeader::FrequencyType::value_type ValueType;
    stream.read(reinterpret_cast<char*>(const_cast<double*>(&var.value())), sizeof(ValueType));
    return stream.gcount();
}

// write
template<typename T>
unsigned write_var(std::ostream& stream, T const& var) {
    stream.write(reinterpret_cast<const char*>(&var), sizeof(T));
    return sizeof(T);
}

// sigproc format string types are preceded by the string length
// read
unsigned get_var(std::istream& stream, std::string& var) {
    // read in the size of the string
    int32_t size=0;
    stream.read(reinterpret_cast<char*>(&size), sizeof(size));
    auto s = sizeof(int32_t);

    if( size < 0 || size > 80 ) {
        panda::Error e("SigProc::read: illegal size of string: ");
        e << size;
        throw e;
    }
    
    if(!stream.fail()) {
        stream.read(_buffer, size);
        s += stream.gcount();
        var.assign(_buffer, size); // unnessasary copy we could get rid of if reqd
    }
    return s;
}

// write
template<>
unsigned write_var(std::ostream& stream, std::string const& var) {

    int32_t size=static_cast<int32_t>(var.size());
    if( size < 0 || size > 80 ) {
        panda::Error e("SigProc::write: illegal size of string: ");
        e << size;
        throw e;
    }
    // write out the size of the string
    unsigned s = write_var(stream, size);
    stream.write(var.data(), size);
    return size + s;
}    

std::istream& operator>>(std::istream& stream, SigProcLabel& var)
{
    get_var(stream, static_cast<std::string&>(var));
    return stream;
}

std::ostream& operator<<(std::ostream& stream, SigProcLabel const& var)
{
    write_var(stream, static_cast<std::string const&>(var));
    return stream;
}
*/
                        
SigProcHeader::SigProcHeader()
{
    reset();
}

SigProcHeader::~SigProcHeader()
{
}

void SigProcHeader::start_time(typename utils::ModifiedJulianClock::time_point const& timestamp)
{
    //_tstart = timestamp.time_since_epoch().count();
    this->tstart(timestamp);
}

std::vector<SigProcHeader::FrequencyType> const& SigProcHeader::frequency_channels() const
{
    return BaseT::frequency_channels();
}

void SigProcHeader::frequency_channels(std::vector<SigProcHeader::FrequencyType> const& freqs)
{
    BaseT::frequency_channels(freqs);
    if(freqs.size())
        number_of_channels(freqs.size());
}

typename utils::ModifiedJulianClock::time_point SigProcHeader::start_time() const {
    if(this->tstart().is_set()) return *this->tstart();
    return utils::ModifiedJulianClock::time_point();
}

std::ostream& operator<<(std::ostream& os, SigProcHeader const& h)
{
    os << pss::astrotypes::sigproc::Header::Info() << h;
    //   << "\tstart_time      : " << std::setprecision(std::numeric_limits<double>::digits10 + 1) << h.start_time()  << "\n"
    return os;
}

} // namespace sigproc
} // namespace cheetah
} // namespace ska
