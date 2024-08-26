/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2024 The SKA organisation
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
#ifndef SKA_CHEETAH_GENERATORS_SIMPLEPULSAR_H
#define SKA_CHEETAH_GENERATORS_SIMPLEPULSAR_H


#include "DataGenerator.h"
#include "cheetah/data/TimeFrequencyCommon.h"
#include "cheetah/corner_turn/CornerTurn.h"
#include "cheetah/data/DmConstant.h"
#include <random>

namespace ska {
namespace cheetah {
namespace generators {
class SimplePulsarConfig;

/**
 * @brief A simple pulsar generator with no acceleration at the moment.
 *
 * @details The generator takes in mean, standard deviation, period, pulse width and DM as inputs.
 *
 */

template<typename DataType>
class SimplePulsar : public DataGenerator<DataType>
{
        typedef DataGenerator<DataType> BaseT;
        typedef typename DataType::NumericalRep NumericalRep;
        typedef boost::units::quantity<data::dm_constant::s_mhz::Unit, double> DmConstantType;

    public:
        SimplePulsar(SimplePulsarConfig const&);
        SimplePulsar(SimplePulsar&&) = default;
        ~SimplePulsar();

        /**
        * @brief simulate next block.
        */
        virtual void next(DataType& data) override;

        /**
        * @brief return selected mean.
        */
        float mean();

        /**
        * @brief generate a pulse template.
        */
        void generate_pulse(DataType& data);

        /**
        * @brief add pulse to the TF data.
        */
        typename data::EnableIfIsTimeFrequency<DataType, void> add_pulsar(DataType& data, std::vector<float>& noise_data);

        /**
        * @brief compute DM delay.
        */
        long double dmdelay(long double f1, long double f2, float dm, float tsamp);

    private:
        SimplePulsarConfig const& _config;
        static std::random_device _dev;
        std::mt19937 _generator;
        std::normal_distribution<float> _rand;
        std::vector<float> _gaussian_pulse;
        std::vector<unsigned> _shifts;
        unsigned int _start_sample_number;
        unsigned int _samples_per_period;
        DmConstantType _dm_constant;

};


} // namespace generators
} // namespace cheetah
} // namespace ska
#include "detail/SimplePulsar.cpp"

#endif // SKA_CHEETAH_GENERATORS_SIMPLEPULSAR_H
