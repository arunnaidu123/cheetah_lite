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
#ifndef SKA_CHEETAH_GENERATORS_PROFILEMANAGER_H
#define SKA_CHEETAH_GENERATORS_PROFILEMANAGER_H

#include "cheetah/generators/pulse_profile/PulsarProfile.h"
#include "cheetah/generators/pulse_profile/PulsarProfileId.h"
#include "cheetah/data/Units.h"

#include <map>
#include <list>

namespace ska {
namespace cheetah {
namespace generators {

class PulsarProfileConfig;

/**
 * @brief
 *   An API for accessing pulsar pulse profiles
 * @details
 */

class ProfileManager
{
    public:
        typedef boost::units::quantity<data::MilliSeconds, double> PeriodUnitType;

    public:
        ProfileManager();
        ~ProfileManager();

        /**
         * @brief return a profile of a known pulsar
         */
        PulsarProfile profile(PulsarProfileId const&) const ;
        PulsarProfile profile(PulsarProfileConfig const&);

        /**
         * @brief return the list of known pulsar profiles
        std::list<PulsarProfileId> const& profile_list() const;
         */

        /**
         * @brief add a profile to the mamanger
         */
        void add(PulsarProfileId id, PulsarProfile profile);

    private:
        std::map<PulsarProfileId, PulsarProfile> _profiles;
        std::list<PulsarProfileId> _pulsar_id;
};


} // namespace generators
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_GENERATORS_PROFILEMANAGER_H
