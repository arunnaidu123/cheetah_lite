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
#ifndef SKA_CHEETAH_UTILS_BINMAP_H
#define SKA_CHEETAH_UTILS_BINMAP_H


namespace ska {
namespace cheetah {
namespace utils {

/**
 * @brief
 *     Explicitly map a range on to a set of constant width indexed data bins
 *
 * @details
 *     This class provides only scaling details, and does not actually store any data.
 *     The index should be used to map to a suitable container
 *
 * index:         0                1                2
 *       ]-----------------]----------------]----------------]
 * lower_bound       upper_bound+width                   upper_bound
 *
 *
 */

template<typename DataType>
class BinMap
{
    public:
        BinMap();
        BinMap(unsigned int number_of_bins);
        /**
         * @brief construct a binning map,
         * @param start the lower_bound
         * @param end the upper_bound
         */
        BinMap(unsigned int number_of_bins, DataType start, DataType const& end);
        ~BinMap();

        /**
         * @brief reset the number of bins
         * @param number_of_bins The new number of bins
         */
        void reset(unsigned int number_of_bins);

        /// set the assignemnt value for the first bin (n.b. not the same as the lower limit of the first bin)
        void set_lower_bound(DataType);

        /// set the the lower bound for first bin
        void set_bounds(DataType lower, DataType upper);

        //  @brief assumes start is set
        //  @details sets bin_width automatically according to start and number of bins
        void set_upper_bound(DataType const&);

        /// set the width of each bin
        //  @details end frequency will also change
        void set_bin_width(DataType);

        inline DataType upper_bound() const;
        inline DataType lower_bound() const;

        /**
         * @brief give the bin Index of the value.
         * @details n.b no range checks are made
         * so passing endValue() will return an invalid index
         */
        unsigned int bin_index(DataType const&) const;

        /**
         * @brief the width of a the bin (all are the same width)
         */
        DataType bin_width() const;

        /**
         * @brief the total number of bins
         */
        unsigned int number_of_bins() const;

        // the value represented by the left hand edge of the bin
        DataType bin_start(unsigned int index) const;

        // the value represented by the right hand edge of the bin
        DataType bin_end(unsigned int index) const;

        // return the value associated with the bin with the specified index
        inline DataType bin_assignment_number(int index) const;

        // return the index of the last bin
        inline unsigned last_bin_index() const;

        // return the assignment number of the last bin
        DataType last_bin_assignment_value() const;
        DataType first_bin_assignment_value() const;

        bool equal(const BinMap<DataType>& map_1) const;

    private:
        unsigned int _n_bins;
        DataType _lower;
        DataType _width;
        DataType _halfwidth;

};

template<typename DataType>
bool operator==(const BinMap<DataType>& map_1, const BinMap<DataType>& map_2);

} // namespace utils
} // namespace cheetah
} // namespace ska
#include "cheetah/utils/detail/BinMap.cpp"

#endif // SKA_CHEETAH_UTILS_BINMAP_H
