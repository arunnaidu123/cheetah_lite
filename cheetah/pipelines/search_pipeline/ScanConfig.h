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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_SCANCONFIG_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_SCANCONFIG_H


#include "cheetah/utils/Config.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/DedispersionMeasure.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

/**
 * @brief Class of all common input parameters
 *
 * @details
 *
 */
class ScanConfig : public cheetah::utils::Config
{
    public:
        typedef typename boost::units::make_scaled_unit<boost::units::si::time, boost::units::scale<10, boost::units::static_rational<-6>>>::type MicroSeconds;
        typedef boost::units::quantity<MicroSeconds, double> MicroSecTimeType;
        typedef data::DedispersionMeasureType<double> Dm;
        typedef boost::units::quantity<boost::units::si::time, int> ScanTimeType;
    public:
        ScanConfig();
        ~ScanConfig();

        /**
         * @brief Returns acceleration search range (+/-)
         */
        data::AccelerationType accel_range() const;

        /**
         * @brief Returns Beam Band width in MHz
         */
        data::FrequencyType beam_bw() const;

        /**
         * @brief Returns Beam Identifier
         */
        int beam_id() const;

        /**
         * @brief Returns the number of bits per every sample
         */
        int bit_per_sample() const;

        /**
         * @brief Returns the Dispersion measure in pc cm-3
         */
        Dm disp_measure() const;

        /**
         * @brief Returns the number of Frequency channels
         */
        int freq_channels() const;

        /**
         * @brief Returns the global scan Identifier
         */
        int scan_id() const;

        /**
         * @brief Teturns the global param scanTime in sec.
         */
        ScanTimeType scan_time() const;

        /**
         * @brief Returns the Sub-array Identifier
         */
        int sub_array_id() const;

        /**
         * brief Returns the Data sampling time in usec.
         */
        MicroSecTimeType time_resolution() const;

        /**
         * @brief Returns the time samples in each blocks
         */
        uint64_t time_samples() const;

        /*
         * @brief Returns the maximun number of trials
         */
        int trials_number() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        int _beam_id;                           /**< the beam identifier number         */
        int _bit_per_sample;                    /**< the number of bits/sample          */
        int _freq_channels;                     /**< the number of channels in freq.    */
        int _scan_id;                           /**< the scan identifier number         */
        int _sub_array_id;                      /**< the sub-array identifier number    */
        int _trials_number;                     /**< the max number of trials           */
        uint64_t _time_samples;                 /**< the number of sample in a block    */
        data::AccelerationType _accel_range;    /**< acceleration range in m/s/s        */
        data::FrequencyType _beam_bw;           /**< frequency beam width in MHz        */
        Dm _disp_measure;                       /**< the dispersion measure in pc cm-3  */
        ScanTimeType _scan_time;                /**< the total scan time in sec.        */
        MicroSecTimeType _time_resolution;      /**< the sampling time in microsec      */
};


} // namespace search_pipeline
} // namespace pipelines
} // namespaceClasscheetah
} // namespace ska

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_SCANCONFIG_H