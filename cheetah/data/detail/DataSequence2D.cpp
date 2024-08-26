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

template <typename T>
DataSequence2D<Cpu,T>::DataSequence2D(){}

template <typename T>
DataSequence2D<Cpu,T>::DataSequence2D(std::size_t fast_axis_length, std::size_t slow_axis_length)
    : _fast_axis_length(fast_axis_length)
    , _slow_axis_length(slow_axis_length)
    , _data()
{
    this->_data.resize(fast_axis_length*slow_axis_length);
}

template <typename T>
DataSequence2D<Cpu,T>::DataSequence2D(std::size_t fast_axis_length, std::size_t slow_axis_length, const T &fill)
    : _fast_axis_length(fast_axis_length)
    , _slow_axis_length(slow_axis_length)
    , _data(fast_axis_length*slow_axis_length,fill)
{
}

template <typename T>
DataSequence2D<Cpu,T>::~DataSequence2D(){}

template <typename T>
std::size_t DataSequence2D<Cpu,T>::fast_axis_length() const { return this->_fast_axis_length;}

template <typename T>
std::size_t DataSequence2D<Cpu,T>::slow_axis_length() const { return this->_slow_axis_length;}

template <typename T>
std::size_t DataSequence2D<Cpu,T>::size() const { return this->_data.size();}

template <typename T>
void DataSequence2D<Cpu,T>::resize(std::size_t fast_axis_length, std::size_t slow_axis_length, const T &fill)
{
    this->_fast_axis_length = fast_axis_length;
    this->_slow_axis_length = slow_axis_length;
    this->_data.resize(this->_fast_axis_length*this->_slow_axis_length);
    std::fill(this->_data.begin(),this->_data.end(), fill);
}

template <typename T>
void DataSequence2D<Cpu,T>::resize(std::size_t fast_axis_length, std::size_t slow_axis_length)
{
    this->_fast_axis_length = fast_axis_length;
    this->_slow_axis_length = slow_axis_length;
    this->_data.resize(this->_fast_axis_length*this->_slow_axis_length);
}

template <typename T>
T const* DataSequence2D<Cpu,T>::data() const {return &(this->_data[0]);}

template <typename T>
T* DataSequence2D<Cpu,T>::data() {return &(this->_data[0]);}

template <typename T>
typename DataSequence2D<Cpu,T>::Iterator DataSequence2D<Cpu,T>::begin() {
    return Iterator(*this);
}

template <typename T>
typename DataSequence2D<Cpu,T>::ConstIterator DataSequence2D<Cpu,T>::begin() const {
    return ConstIterator(*this);
}

template <typename T>
typename DataSequence2D<Cpu,T>::ConstIterator DataSequence2D<Cpu,T>::cbegin() const {
    return ConstIterator(*this);
}


template <typename T>
typename DataSequence2D<Cpu,T>::Iterator DataSequence2D<Cpu,T>::end() {
    return Iterator(*this)+this->_data.size();
}

template <typename T>
typename DataSequence2D<Cpu,T>::ConstIterator DataSequence2D<Cpu,T>::end() const {
    return ConstIterator(*this)+this->_data.size();
}

template <typename T>
typename DataSequence2D<Cpu,T>::ConstIterator DataSequence2D<Cpu,T>::cend() const {
    return ConstIterator(*this)+this->_data.size();
}

template <typename T>
typename DataSequence2D<Cpu,T>::PartialIterator DataSequence2D<Cpu,T>::slice(std::size_t fbegin,
        std::size_t fend,
        std::size_t sbegin,
        std::size_t send){

    BOOST_ASSERT(fend <= this->fast_axis_length());
    BOOST_ASSERT(send <= this->slow_axis_length());

    return DataSequence2DPartialIterator<Cpu,T>(*this, fbegin,fend,sbegin,send);
}

template<typename Arch, typename DataType>
bool operator==(DataSequence2D<Arch, DataType> const& a, DataSequence2D<Arch, DataType> const& b)
{
    if( !(a.fast_axis_length() == b.fast_axis_length()
        && a.slow_axis_length() == b.slow_axis_length()) )
    {
        return false; // different dimensions
    }
    auto it_a = a.cbegin();
    auto it_b = b.cbegin();

    while(it_a != a.cend()) {
        if(*it_a != *it_b) return false; // values differ
        ++it_a;
        ++it_b;
    }
    return true;
}

} // namespace data
} // namespace cheetah
} // namespace ska



