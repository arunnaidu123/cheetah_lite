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
#ifndef SKA_CHEETAH_UTILS_TIMEPOINT_H
#define SKA_CHEETAH_UTILS_TIMEPOINT_H
#include <chrono>
#include <ostream>


namespace ska {
namespace cheetah {
namespace utils {

/**
 * @brief
 *     extension of std::chrono::time_point for chhetah clocks
 * @tparam ClockType must support the std::Clock conecpt as as weel as defining a duration type @member diff_from_system_epoch
 */
template<typename ClockType, typename Duration = typename ClockType::duration>
struct TimePoint : public std::chrono::time_point<ClockType, Duration>
{
    explicit TimePoint( const Duration& d = typename ClockType::duration() );
    explicit TimePoint( const std::chrono::system_clock::time_point& tp );
    TimePoint( const std::chrono::time_point<ClockType, Duration>& tp );
    TimePoint( std::chrono::time_point<ClockType, Duration>&& );

    operator typename std::chrono::system_clock::time_point() const;
    TimePoint<ClockType, Duration>& operator+=(Duration const&);
    TimePoint<ClockType, Duration>& operator-=(Duration const&);

    /**
     * @brief convert to a C style time struct.
     * @details very useful if you want to output the time as a string with e.g. std::put_time
     */
    std::time_t to_time_t() const;
};

template<typename ClockType, typename Duration, typename Duration2>
TimePoint<ClockType, Duration> operator+(TimePoint<ClockType, Duration> const& lhs, Duration2 const& rhs);

template<typename ClockType, typename Duration, typename Duration2>
TimePoint<ClockType, Duration> operator-(TimePoint<ClockType, Duration> const& lhs, Duration2 const& rhs);

template<typename ClockType, typename Duration, typename Duration2>
Duration operator-(TimePoint<ClockType, Duration> const& lhs, TimePoint<ClockType, Duration> const& rhs);

/**
 * @brief output a TimePoint as a string to the ostream
 * @details in ISO8601 format UTC. For other formats you must
 *          use the to_time_t() method and the std::put_time function
 */
template<typename ClockType, typename Duration>
std::ostream& operator<<(std::ostream&, TimePoint<ClockType> const& tp);

} // namespace utils
} // namespace cheetah
} // namespace ska
#include "cheetah/utils/detail/TimePoint.cpp"

#endif // SKA_CHEETAH_UTILS_TIMEPOINT_H
