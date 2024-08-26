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
#ifndef SKA_CHEETAH_GENERATORS_PULSARPROFILE_H
#define SKA_CHEETAH_GENERATORS_PULSARPROFILE_H

#include <vector>
#include <utility>

namespace ska {
namespace cheetah {
namespace generators {

/**
 * @brief
 *    A simple intensity profile representing a pulse shape 
 * 
 * @details
 *    Described as points in equally spaced phase
 * 
 */

class PulsarProfile
{
    public:
        typedef double ProfileDataPoint;

    private:
        typedef std::vector<ProfileDataPoint> ProfileContainer;

    public:
        typedef typename ProfileContainer::iterator Iterator;
        typedef typename ProfileContainer::const_iterator ConstIterator;

    public:
        PulsarProfile();
        PulsarProfile(std::initializer_list<double>);
        ~PulsarProfile();
    
        /**
         * @brief the start of the profile spectrum
         */
        Iterator begin();
        ConstIterator cbegin() const;

        /**
         * @brief the end of the profile spectrum
         */
        Iterator end();
        ConstIterator cend() const;

        /**
         * @brief add a value to the profile
         * @details value will be inserted at the next frequency range
         */
        void add(double);

        /**
         * @brief return the sum of all frequencies
         */
        double sum() const;

        /**
         * @brief return the number of data points in the profile
         */
        std::size_t size() const;

    private:
        ProfileContainer _profile;
};

PulsarProfile& operator<<(PulsarProfile& p, double value);

} // namespace generators
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_GENERATORS_PULSARPROFILE_H 
