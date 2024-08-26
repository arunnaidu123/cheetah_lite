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
#ifndef SKA_CHEETAH_DATA_BIRDIE_H
#define SKA_CHEETAH_DATA_BIRDIE_H

#include "cheetah/data/Units.h"

namespace ska {
namespace cheetah {
namespace data {


/**
 * @brief      Class for storing birdies (known RFI periodicities/frequencies)
 * 
 * @details     Each birdie represents a signal of a given width to be excised from
 *             the spectrum of each DM trial during a search for periodic pulsar-like
 *             signals
 */
class Birdie
{
public:

    /**
     * @brief      Create an empty Birdie instance
     */
    Birdie();

    /**
     * @brief      Create a new Birdie
     *
     * @param      frequency  The central frequency of the birdie
     * @param      width      The full width of the birdie (centered around the frequency)
     */
    Birdie(data::FourierFrequencyType const& frequency, data::FourierFrequencyType const& width);
    Birdie(Birdie const&) = default;
    Birdie(Birdie&&) = default;
    ~Birdie();

    /**
     * @brief      Copy assignment
     */
    Birdie& operator=(Birdie const&) = default;

    /**
     * @brief      Set the frequency of the birdie
     *
     * @param      frequency  The frequency
     */
    void frequency(data::FourierFrequencyType const& frequency);

    /**
     * @brief      Get the frequency of the birdie
     *
     * @return     The frequency of the birdie in Hz
     */
    data::FourierFrequencyType const& frequency() const;

    /**
     * @brief      Set the width of the birdie
     *
     * @param      width  The width in Hz
     */
    void width(data::FourierFrequencyType const& width);

    /**
     * @brief      Get the width of the birdie
     *
     * @return     The frequency width of the birdie in Hz
     */
    data::FourierFrequencyType const& width() const;

private:
    data::FourierFrequencyType _frequency;
    data::FourierFrequencyType _width;
};


} // namespace data
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_DATA_BIRDIE_H 
