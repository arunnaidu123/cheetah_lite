/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2023 The SKA organisation
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

#ifndef SKA_CHEETAH_MODULES_DDTR_KLOTSKI_BRUTEFORCE_DEDISPERSIONSTRATEGY_H
#define SKA_CHEETAH_MODULES_DDTR_KLOTSKI_BRUTEFORCE_DEDISPERSIONSTRATEGY_H

#include "cheetah/modules/ddtr/Config.h"
#include "cheetah/modules/ddtr/DedispersionTrialPlan.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/TimeFrequency.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski_bruteforce {

/**
 * @brief Dedipsersion strategy for the klotski_bruteforce module
 * @details computes the number of subbands and the klotski_bruteforces per subband required for the algorithm
 */
template<typename NumericalT>
class DedispersionStrategy
{
    private:
        typedef NumericalT NumericalRep;
        typedef boost::units::quantity<data::MegaHertz, double> FrequencyType;
        typedef data::DedispersionMeasureType<float> Dm;
        typedef boost::units::quantity<data::dm_constant::s_mhz::Unit, double> DmConstantType;
        typedef boost::units::quantity<boost::units::si::time, double> TimeType;
        typedef std::vector<std::vector<std::vector<unsigned int>>> ArrayType;

    public:
        DedispersionStrategy(data::TimeFrequency<Cpu, NumericalRep> const& chunk
                            , ddtr::Config const& config
                            , std::size_t cpu_memory);
        DedispersionStrategy(DedispersionStrategy const&) = delete;
        ~DedispersionStrategy();

        /**
         * @brief The number of dm ranges
         */
        unsigned int number_of_dm_ranges() const;

        /**
         * @brief vector of input bin sizes
         */
        std::vector<unsigned int> const& in_bin() const;

        /**
         * @brief Value used to make sure that dms from dm_low to dm_high are used
         */
        unsigned int maxshift() const;

        /**
         * @brief An array containing the lowest bound of each dm range
         */
        std::vector<Dm> const& dm_low() const;

        /**
         * @brief An array containing the highest bound of each dm range
         */
        std::vector<Dm> const& dm_high() const;

        /**
         * @brief An array containing the step size of each dm range
         */
        std::vector<Dm> const& dm_step() const;

        /**
         * @brief An array containing a constant associated with each channel to perform dedispersion algorithm
         */
        std::vector<float> const& dmshifts() const;

        /**
         * @brief An array containing a constant associated with each channel to perform dedispersion algorithm
         */
        std::vector<std::vector<unsigned int>> const& start_dmshifts() const;

        /**
         * @brief An array containing the number of dms for each range
         */
        std::vector<unsigned int> const& ndms() const ;

        /**
         * @brief The total number of dm
         */
        unsigned int total_ndms() const;

        /**
         * @brief The highest dm value
         */
        Dm max_dm() const;

        /**
         * @brief Time sample value
         */
        TimeType tsamp() const;

        /**
         * @brief The number of time samples
         */
        unsigned int nsamps() const;

        /**
         * @brief The number of frequency channels
         */
        unsigned int nchans() const;

        /**
         * @brief The number of frequency channels per klotski_bruteforce
         */
        unsigned int kchans() const;

        /**
         * @brief The number of samples per klotski_bruteforce
         */
        unsigned int ksamps() const;

        /**
         * @brief A two dimensional vector containging the information about the
         * number of klotski_bruteforces per band needed for the dedispersion of the
         * FT block.
         */
        ArrayType const& kloskis_per_band() const;

        /**
         * @brief returns dedispersed samples per DM in dmtrials
         */
        data::DimensionSize<data::Time> dedispersed_samples();

        /**
         * @brief adjust the strategy with the updates variables
         */
        void resize(size_t const number_of_samples, size_t const cpu_memory);

        /**
         * @brief return pointer to temp_work_area which hold the intermediate results mostly the FT data
         */
        std::shared_ptr<std::vector<unsigned short>> temp_work_area();

        /**
         * @brief returns pointer to subanded_dm_trials shich holds the subbanded dedispersed data
         */
        std::shared_ptr<std::vector<std::vector<int>>> subanded_dm_trials();

    private:
        /**
         * @brief Computes the dedispersion strategy
         */
        void make_strategy(size_t cpu_memory);

    private:
        std::size_t _cpu_memory;
        ArrayType _kloskis_per_band;
        std::shared_ptr<std::vector<unsigned short>> _temp_work_area;
        std::shared_ptr<std::vector<std::vector<int>>> _subanded_dm_trials;
        unsigned int _number_of_dm_ranges;
        std::vector<Dm> _dm_low;
        std::vector<Dm> _dm_high;
        std::vector<Dm> _dm_step;
        std::vector<unsigned int> _in_bin;
        unsigned int _maxshift;
        std::vector<float> _dmshifts;
        std::vector<std::vector<unsigned int>> _start_dmshifts;
        std::vector<unsigned int> _ndms;
        unsigned int _total_ndms;
        Dm _max_dm;
        TimeType _tsamp;
        unsigned int _cache_size;
        unsigned int _nsamps;
        unsigned int _nchans;
        unsigned int _kchans;
        FrequencyType _fch1;
        FrequencyType _foff;
        std::size_t _dedispersed_time_samples;
        DmConstantType _dm_constant;
};

} // namespace klotski_bruteforce
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#include "cheetah/modules/ddtr/klotski_bruteforce/detail/DedispersionStrategy.cpp"

#endif // SKA_CHEETAH_MODULES_DDTR_KLOTSKI_BRUTEFORCE_DEDISPERSIONSTRATEGY_H
