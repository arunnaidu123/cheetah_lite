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
#ifndef SKA_CHEETAH_UTILS_JULIANCLOCK_H
#define SKA_CHEETAH_UTILS_JULIANCLOCK_H

#include "cheetah/utils/detail/TimePoint.h"
#include "pss/astrotypes/units/JulianClock.h"
#include <chrono>

namespace ska {
namespace cheetah {
namespace utils {

/**
 * @brief representation of julian time is in days
 */
typedef pss::astrotypes::units::julian_day julian_day;
typedef pss::astrotypes::units::julian_day julian_days;

/**
 * @brief
 *    a std::chrono compatible clock representing the Julian Clock
 * @details
 *    deprecated: please use pss::astotype::utils::JulianClock
 */
using JulianClock = pss::astrotypes::units::JulianClock;


} // namespace utils
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_UTILS_JULIANCLOCK_H 
