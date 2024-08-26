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
#include "cheetah/data/TimeFrequencyContainer.h"
#include "panda/MultiIterator.h"


namespace ska {
namespace cheetah {
namespace data {
namespace {

template<class TimeFrequencyType>
class TimeFrequencySampleIterator {
        typedef typename TimeFrequencyType::ConstSpectra IteratorType;

    public:
        TimeFrequencySampleIterator() // required for multiIterator
            : _sample_number(0)
        {
        }

        TimeFrequencySampleIterator(TimeFrequencyType& tf) // end iterator constructor
            : _sample_number(tf.number_of_spectra())
            , _it(new IteratorType(tf.spectrum(_sample_number)))
        {
        }

        TimeFrequencySampleIterator(TimeFrequencyType& tf, std::size_t sample_number)
            : _tf(tf.shared_from_this())
            , _sample_number(sample_number)
            , _it(new IteratorType(tf.spectrum(sample_number)))
        {
        }


        bool operator==(TimeFrequencySampleIterator const& o) const
        {
            if(_tf == o._tf) {
                return _sample_number == o._sample_number;
            }
            if(!_tf) { // local _tf is null
                return o._sample_number == o._tf->number_of_spectra();
            }
            if(!o._tf) { // o._tf is null
                return _sample_number == _tf->number_of_spectra();
            }
            return false; // o and t refere to different tf objects
        }

        bool operator!=(TimeFrequencySampleIterator const& o) const
        {
            return !(o==*this);
        }

        typename TimeFrequencyType::ConstSpectra& operator*() const
        {
            return *_it;
        }

        TimeFrequencySampleIterator& operator++()
        {
            *_it = this->_tf->spectrum(++this->_sample_number);
            return *this;
        }

        TimeFrequencySampleIterator operator++(int)
        {
            TimeFrequencySampleIterator tmp(*this);
            ++*this;
            return tmp;
        }

    protected:
        std::shared_ptr<TimeFrequencyType> _tf;
        std::size_t _sample_number;
        mutable typename std::shared_ptr<IteratorType> _it;
};

template<typename T, typename  TimeFrequencyType, class ChannelIterationEndHandler, class TimeDiscontinuityHandler>
struct MultiIteratorDepthTraitsImpl : panda::DefaultIteratorDepthTraits<T>
{
};

template<typename TimeFrequencyType, class ChannelIterationEndHandler, class TimeDiscontinuityHandler>
struct MultiIteratorDepthTraitsImpl<typename TimeFrequencyType::ConstSpectra::const_iterator, TimeFrequencyType, ChannelIterationEndHandler, TimeDiscontinuityHandler>
    : public panda::IteratorDepthTraits<typename TimeFrequencyType::ConstSpectra::const_iterator, ChannelIterationEndHandler>
{
    typedef panda::IteratorDepthTraits<typename TimeFrequencyType::ConstSpectra::const_iterator, ChannelIterationEndHandler>  BaseT;
    MultiIteratorDepthTraitsImpl()
    {}
    //using BaseT::IteratorDepthTraits;
};

template<typename TimeFrequencyType, class ChannelIterationEndHandler, class TimeDiscontinuityHandler>
struct MultiIteratorDepthTraitsImpl<typename TimeFrequencyType::ConstSpectra, TimeFrequencyType, ChannelIterationEndHandler, TimeDiscontinuityHandler>
    : public panda::IteratorDepthTraits<typename TimeFrequencyType::ConstSpectra, ChannelIterationEndHandler>
{
    typedef panda::IteratorDepthTraits<typename TimeFrequencyType::ConstSpectra, ChannelIterationEndHandler>  BaseT;
    MultiIteratorDepthTraitsImpl(ChannelIterationEndHandler ch_handler)
        : BaseT(ch_handler) {}
    //using BaseT::IteratorDepthTraits;
};


// specialisation for TimeFrequency objects
template<typename TimeFrequencyType, class ChannelIterationEndHandler, class TimeDiscontinuityHandler>
struct MultiIteratorDepthTraitsImpl<const std::shared_ptr<TimeFrequencyType>, TimeFrequencyType, ChannelIterationEndHandler, TimeDiscontinuityHandler>
    : public panda::IteratorDepthTraits<const std::shared_ptr<TimeFrequencyType>>
{
        typedef TimeFrequencySampleIterator<const TimeFrequencyType> Iterator; // can be the end iterator

        MultiIteratorDepthTraitsImpl(TimeDiscontinuityHandler td_handler)
            : _td_handler(td_handler)
            , _expected_time(TimeFrequencyType::TimePointType::min())
            , _last_interval(0.0 * boost::units::si::seconds)
        {
        }

        Iterator begin(TimeFrequencyType const& tf)
        {
            // check the time is as expected, or call the handler
            auto const& interval = tf.sample_interval();
            if(((tf.start_time() - _expected_time) >= 0.5 * interval) || interval != _last_interval)
            {
                _td_handler(tf.start_time(), tf.sample_interval());
            }
            _expected_time = tf.end_time() + interval;
            _last_interval = interval;
            return Iterator(tf, 0);
        }

        Iterator begin(std::shared_ptr<TimeFrequencyType> const& tf)
        {
            if(tf) return begin(*tf);
            throw panda::Error("shared_ptr not initialised");
        }

        Iterator end(TimeFrequencyType const& tf)
        {
            return Iterator(tf);
        }

        Iterator end(std::shared_ptr<TimeFrequencyType> const& tf)
        {
            return end(*tf);
        }

    private:
        TimeDiscontinuityHandler _td_handler;
        typename TimeFrequencyType::TimePointType _expected_time;
        typedef typename TimeFrequencyType::TimePointType::rep Rep;
        typename TimeFrequencyType::TimeType  _last_interval;
};

template<typename TimeFrequencyType, class ChannelIterationEndHandler, class TimeDiscontinuityHandler>
struct MultiIteratorDepthTraits
{
    template <typename T> using DepthTraits = MultiIteratorDepthTraitsImpl<T, TimeFrequencyType, ChannelIterationEndHandler, TimeDiscontinuityHandler>;
};

} // namespace

template <class Arch, typename NumericalT>
struct TimeFrequencyContainer<Arch, NumericalT>::ConstEndChannelIterator
{
};

template <class Arch, typename NumericalT>
template <class ChannelIterationEndHandler, class TimeDiscontinuityHandler>
struct TimeFrequencyContainer<Arch, NumericalT>::ConstChannelIterator
    : public panda::MultiIterator<panda::MultiIteratorTraits
                                    < const decltype(TimeFrequencyContainer<Arch, NumericalT>::_data)
                                    , decltype(TimeFrequencyContainer<Arch, NumericalT>::_data.cbegin())
                                    , MultiIteratorDepthTraits
                                      <typename TimeFrequencyContainer<Arch, NumericalT>::TimeFrequencyType
                                      , ChannelIterationEndHandler
                                      , TimeDiscontinuityHandler
                                      >::template DepthTraits
                                    >
                                >
{
    typedef decltype(TimeFrequencyContainer<Arch, NumericalT>::_data) ContainerType;
    typedef panda::MultiIterator<panda::MultiIteratorTraits
                                    < const ContainerType
                                    , decltype(TimeFrequencyContainer<Arch, NumericalT>::_data.cbegin())
                                    , MultiIteratorDepthTraits
                                      <typename TimeFrequencyContainer<Arch, NumericalT>::TimeFrequencyType
                                      , ChannelIterationEndHandler
                                      , TimeDiscontinuityHandler
                                      >::template DepthTraits
                                    >
                                > BaseT;

    ConstChannelIterator(TimeFrequencyContainer const& ts, ChannelIterationEndHandler ch_handler, TimeDiscontinuityHandler td_handler)
        : BaseT(ts._data.begin(), ts._data.end()
        , MultiIteratorDepthTraitsImpl<typename TimeFrequencyType::ConstSpectra, TimeFrequencyType, ChannelIterationEndHandler, TimeDiscontinuityHandler>(ch_handler)
        , MultiIteratorDepthTraitsImpl<const std::shared_ptr<TimeFrequencyType>, TimeFrequencyType, ChannelIterationEndHandler, TimeDiscontinuityHandler>(td_handler))
    {
    }

    // the end iterator constructor
    ConstChannelIterator(TimeFrequencyContainer const& ts)
        : BaseT(ts._data.begin(), ts._data.end()
        , MultiIteratorDepthTraitsImpl<typename TimeFrequencyType::ConstSpectra, TimeFrequencyType, ChannelIterationEndHandler, TimeDiscontinuityHandler>(ChannelIterationEndHandler())
        , MultiIteratorDepthTraitsImpl<const std::shared_ptr<TimeFrequencyType>, TimeFrequencyType, ChannelIterationEndHandler, TimeDiscontinuityHandler>(TimeDiscontinuityHandler()))
    {
    }

    template<class ChannelEndHandler, class TimeDiscoHandler>
    bool operator==(ConstChannelIterator<ChannelEndHandler, TimeDiscoHandler> const& o) const
    {
        return BaseT::operator==(o);
    }

    inline bool operator==(ConstEndChannelIterator) const
    {
        return BaseT::end();
    }

    inline bool operator!=(ConstEndChannelIterator) const
    {
        return !BaseT::end();
    }

    template<class ChannelEndHandler, class TimeDiscoHandler>
    bool operator!=(ConstChannelIterator<ChannelEndHandler, TimeDiscoHandler> const& o) const
    {
        return BaseT::operator!=(o);
    }
};

template <class Arch, typename NumericalT>
TimeFrequencyContainer<Arch, NumericalT>::TimeFrequencyContainer()
{
}

template <class Arch, typename NumericalT>
TimeFrequencyContainer<Arch, NumericalT>::~TimeFrequencyContainer()
{
}

template <class Arch, typename NumericalT>
void TimeFrequencyContainer<Arch, NumericalT>::clear()
{
    _data.clear();
    _number_of_spectra = 0;
}

template <class Arch, typename NumericalT>
bool TimeFrequencyContainer<Arch, NumericalT>::empty() const
{
    return _data.empty();
}

template <class Arch, typename NumericalT>
void TimeFrequencyContainer<Arch, NumericalT>::add(std::shared_ptr<TimeFrequencyType> const& data)
{
    if(!_data.empty()) {
        if(data->number_of_channels() != number_of_channels()) {
            panda::Error e("expecting ");
            e << number_of_channels() << " channels, got " << data->number_of_channels();
            throw e;
        }
    }
    _number_of_spectra += data->number_of_spectra();
    _data.push_back(data);
}

template <class Arch, typename NumericalT>
void TimeFrequencyContainer<Arch, NumericalT>::add(typename TimeFrequencyContainer<Arch, NumericalT>::TimeFrequencyType& data)
{
    this->add(data.shared_from_this());
}

template <class Arch, typename NumericalT>
std::size_t TimeFrequencyContainer<Arch, NumericalT>::number_of_channels() const
{
    return _data[0]->number_of_channels();
}

template <class Arch, typename NumericalT>
std::size_t TimeFrequencyContainer<Arch, NumericalT>::number_of_spectra() const
{
    return _number_of_spectra;
}

template <class Arch, typename NumericalT>
typename TimeFrequencyContainer<Arch, NumericalT>::TimePointType const& TimeFrequencyContainer<Arch, NumericalT>::start_time() const
{
    return _data[0]->start_time();
}

template <class Arch, typename NumericalT>
typename TimeFrequencyContainer<Arch, NumericalT>::TimeType TimeFrequencyContainer<Arch, NumericalT>::sample_interval() const
{
    return _data[0]->sample_interval();
}

template <class Arch, typename NumericalT>
std::vector<typename TimeFrequencyContainer<Arch, NumericalT>::TimeFrequencyType::FrequencyType> const& TimeFrequencyContainer<Arch, NumericalT>::channel_frequencies() const
{
    return _data[0]->channel_frequencies();
}

template<class Arch, typename NumericalT>
template<class ChannelIterationEndHandler, class TimeDiscontinuityHandler>
typename TimeFrequencyContainer<Arch, NumericalT>::template ConstChannelIterator<ChannelIterationEndHandler, TimeDiscontinuityHandler> TimeFrequencyContainer<Arch, NumericalT>::begin(ChannelIterationEndHandler ch_handler, TimeDiscontinuityHandler td_handler) const
{
    return ConstChannelIterator<ChannelIterationEndHandler, TimeDiscontinuityHandler>(*this, ch_handler, td_handler);
}

/*
template<class Arch, typename NumericalT>
TimeFrequencyContainer<Arch, NumericalT>::ConstChannelIterator<> TimeFrequencyContainer<Arch, NumericalT>::end() const
{
    //return EndConstChannelIterator<ConstChannelIterator<>>(*this);
    return EndConstChannelIterator<TimeFrequencyContainer<Arch, NumericalT>, panda::NoOp, panda::NoOp>(_data.end(), panda::NoOp(), panda::NoOp());
}
*/

template<class Arch, typename NumericalT>
typename TimeFrequencyContainer<Arch, NumericalT>::ConstEndChannelIterator TimeFrequencyContainer<Arch, NumericalT>::end() const
{
    return ConstEndChannelIterator();
}

} // namespace data
} // namespace cheetah
} // namespace ska

namespace std {

template<typename TimeFrequencyType>
struct iterator_traits<ska::cheetah::data::TimeFrequencySampleIterator<TimeFrequencyType>>
{
    typedef typename TimeFrequencyType::value_type value_type;
};

} // namespace std
