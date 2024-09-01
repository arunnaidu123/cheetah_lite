/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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

#ifndef SKA_CHEETAH_MODULES_DDTR_KLOTSKI_DEDISPERSIONSTRATEGY_H
#define SKA_CHEETAH_MODULES_DDTR_KLOTSKI_DEDISPERSIONSTRATEGY_H

#include "cheetah/modules/ddtr/Config.h"
#include "cheetah/modules/ddtr/DedispersionTrialPlan.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/TimeFrequency.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski {

/**
 * @brief Dedipsersion strategy for the klotski module
 * @details Computes the number of subbands and the klotskis per subband required for the algorithm.
 *          in addition, it also compute the all the details needed to perform tree-dedispersion.
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
        typedef std::vector<std::vector<std::vector<std::vector<unsigned int>>>> IntArrayType;
        typedef std::vector<std::vector<std::vector<std::vector<float>>>> FloatArrayType;

    public:
        DedispersionStrategy(data::TimeFrequency<Cpu, NumericalRep> const& chunk
                            , ddtr::Config const& config
                            , std::size_t cpu_memory);
        DedispersionStrategy(DedispersionStrategy const&) = delete;
        ~DedispersionStrategy();

        /**
         * @brief returns maximum channels per band
         */
        unsigned int max_channels_per_band() const;

        /**
         * @brief returns number of bands
         */
        unsigned int number_of_bands() const;

        /**
         * @brief returns vector with number of channels per band
         */
        std::vector<unsigned int> const& channels_per_band() const;

        /**
         * @brief returns vector containing the number of klotkis per band
         */
        std::vector<unsigned int> const& klotskis_per_band() const;

        /**
         * @brief returns 3d vector containing the dmshifts per band dimentions range->bands->channel
         */
        std::vector<std::vector<std::vector<unsigned int>>>& dmshifts_per_band();

        /**
         * @brief returns 4d vector containing the dmshifts per klotski dimentions range->bands->klotski->dm
         */
        IntArrayType dmshifts_per_klotski() const;

        /**
         * @brief returns 3d vector containing the dmshifts per band dimentions range->band->klotski
         */
        std::vector<std::vector<std::vector<unsigned int>>> const& dsamps_per_klotski() const;

        /**
         * @brief return pointer to temp_work_area which hold the intermediate results mostly the FT data
         */
        std::shared_ptr<std::vector<unsigned short>> temp_work_area();

        /**
         * @brief returns pointer to subanded_dm_trials which holds the subbanded dedispersed data
         */
        std::shared_ptr<std::vector<std::vector<int>>> subanded_dm_trials();

        /**
         * @brief returns 4D arry containing the information about the base dm index for the corresponding iteration
         */
        IntArrayType total_base() const;

        /**
         * @brief returns 4D arry containing the information about the dm index for the corresponding iteration
         */
        IntArrayType total_index() const;

        /**
         * @brief returns 4D arry containing the information about the shift to be applied for the corresponding iteration
         */
        IntArrayType total_shift() const;

        /**
         * @brief returns 4D arry containing the information about the iterations per klotski
         */
        IntArrayType counts_array() const;

        /**
         * @brief returns dmshifts per channel
         */
        FloatArrayType dmshifts_per_channel() const;

        /**
         * @brief returns start dmshifts
         */
        std::vector<std::vector<unsigned int>> const& start_dm_shifts() const;

        /**
         * @brief The number of dm ranges
         */
        unsigned int number_of_dm_ranges() const;

        /**
         * @brief returns maximum allowed channels per klotski
         */
        unsigned int max_channels_per_klotski() const;

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
         * @brief Value used to make sure that dms from dm_low to dm_high are used
         */
        unsigned int maxshift() const;

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
        Dm const& max_dm() const;

        /**
         * @brief Time sample value
         */
        std::vector<TimeType> tsamp() const;

        /**
         * @brief return L1 cache size
         */
        std::size_t cache_size() const;

        /**
         * @brief The number of time samples
         */
        unsigned int nsamps() const;

        /**
         * @brief The number of frequency channels
         */
        unsigned int nchans() const;

        /**
         * @brief The number of frequency channels
         */
        FrequencyType fch1() const; // fch1 (start frequency)

        /**
         * @brief The number of frequency channels
         */
        FrequencyType foff() const; // foff (channel width)

        /**
         * @brief returns dedispersed samples per DM in dmtrials
         */
        typename data::DimensionSize<data::Time> dedispersed_samples() const;

        /**
         * @brief returns samples in the subbanded dmtrials
         */
        unsigned int const& number_of_dmtrials_samples() const;

        /**
         * @brief adjust the strategy with the updates variables
         */
        void resize(size_t const number_of_samples, size_t const cpu_memory);


    private:
        /**
         * @brief Computes the dedispersion strategy
         */
        void make_strategy(size_t cpu_memory);

        /**
        * @brief Computes base, shifts and index values need for the dedispersion for each channel
        */
        void update_details( int ndms
                           , int& count
                           , std::vector<unsigned int>& base
                           , std::vector<unsigned int>& shifts
                           , std::vector<unsigned int>& index
                           , float dm_shift
                           , std::vector<float>& dm_shift_excess
                           );

        /**
        * @brief Computes stack values (base, shifts and index) for each klotski
        */
        void generate_details_stack(FloatArrayType const& shifts_per_dm
                            , int ndms
                            , IntArrayType& total_base
                            , IntArrayType& total_index
                            , IntArrayType& total_shift
                            , IntArrayType& counts_array
                            , FloatArrayType& excess_per_dm
                            , unsigned int band
                            , unsigned int klotski
                            , unsigned int range
                            );

    private:
        std::size_t _cpu_memory; // Available CPU memory for DDTR
        unsigned int _max_channels_per_band; // maximum number of channels allowed per band
        unsigned int _number_of_bands; //  number of bands
        std::vector<unsigned int> _channels_per_band; //  channels per band
        std::vector<unsigned int> _klotskis_per_band; //  klotskis per band
        std::vector<std::vector<std::vector<unsigned int>>> _dmshifts_per_band; //  2D array containg the dmshift per dmindex per band
        IntArrayType _dmshifts_per_klotski; //  3D array containg the dmshift per dmindex per klotski per band
        std::vector<std::vector<std::vector<unsigned int>>> _dsamps_per_klotski; // 2D array containing the information about the dedispersion samples per klotski per band
        std::shared_ptr<std::vector<unsigned short>> _temp_work_area; // tempory work area which contains the tranformed input data
        std::shared_ptr<std::vector<std::vector<int>>> _subanded_dm_trials; // output area containg the subbanded DMtrial data
        IntArrayType _total_base; // base DM indices for each iteration
        IntArrayType _total_index; // DM indices for each iteration
        IntArrayType _total_shift; // shift for each DM indices for each iteration
        IntArrayType _counts_array; // contains information about the total number of iterations
        FloatArrayType _dmshifts_per_channel; // shifts per DM for each channel it is 3D array split (band,klotski,channel)
        std::vector<std::vector<unsigned int>> _start_dm_shifts;
        unsigned int _number_of_dm_ranges; // number dm ranges
        unsigned int _max_channels_per_klotski; // maximum allowed channels per klotski
        std::vector<Dm> _dm_low; // start dm per range
        std::vector<Dm> _dm_high; // end dm per range
        std::vector<Dm> _dm_step; // dm step per range
        unsigned int _maxshift; // maxshift which is essentially the overlap
        std::vector<unsigned int> _ndms; // number of dms per range
        unsigned int _total_ndms; // total number of DMs
        Dm _max_dm; // maximum DM in the ranges
        std::vector<TimeType> _tsamp; // sampling time
        std::size_t _cache_size; // L1 cache size in bytes
        unsigned int _nsamps; // number of samples
        unsigned int _nchans; // number of channels
        FrequencyType _fch1; // fch1 (start frequency)
        FrequencyType _foff; // foff (channel width)
        std::size_t _dedispersed_samples; // dedispersed time samples
        unsigned int _number_of_dmtrials_samples;
        DmConstantType _dm_constant; // _dm_constant
        FloatArrayType _dmshifts_per_klotski_excess;
        bool _precise;
};

} // namespace klotski
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#include "cheetah/modules/ddtr/klotski/detail/DedispersionStrategy.cpp"

#endif // SKA_CHEETAH_MODULES_DDTR_KLOTSKI_DEDISPERSIONSTRATEGY_H
