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
#ifndef SKA_CHEETAH_GENERATORS_RFISCENARIO_H
#define SKA_CHEETAH_GENERATORS_RFISCENARIO_H


#include "RfiGenerator.h"

namespace ska {
namespace cheetah {
namespace generators {

/**
 * @brief
 *    Collection of RFI scenarios.
 * @details
 *    Each one assigned to a a different number
 *    in the tempalte parameter list.
 */

template<int, typename DataType>
class RfiScenario
{
    RfiScenario() = delete;
};

/**
 * @brief no rfi
 */
template<typename DataType>
class RfiScenario<0, DataType> : public RfiGenerator<DataType> {
        typedef RfiGenerator<DataType> BaseT;
        typedef typename BaseT::FlaggedDataType FlaggedDataType;

    public:
        RfiScenario();
        ~RfiScenario();
        void next(FlaggedDataType& data) override;

        static constexpr char description[] = "no additional RFI";
};

/**
 * @brief ramp decreasing in intensity with frequency, constant across all time scales. This is bit harsh test for the algorithms
 * as this alters the stats (deviations and mean) significantly. It is difficult for any brute force algorithm to find this case.
 */
template<typename DataType>
class RfiScenario<1, DataType> : public RfiGenerator<DataType> {
        typedef RfiGenerator<DataType> BaseT;
        typedef typename BaseT::FlaggedDataType FlaggedDataType;
    public:
        RfiScenario();
        ~RfiScenario();
        void next(FlaggedDataType& data) override;

        static constexpr char description[] = "RFI: signal ramps up with frequency, constant across time";
};

/**
 * @brief peak of RFI in middle of block, with Gaussian wings.
 */
template<typename DataType>
class RfiScenario<2, DataType> : public RfiGenerator<DataType> {
        typedef RfiGenerator<DataType> BaseT;
        typedef typename BaseT::FlaggedDataType FlaggedDataType;
    public:
        RfiScenario();
        ~RfiScenario();
        void next(FlaggedDataType& data) override;
        static constexpr char description[] = "RFI: Gaussian spike in center of the block";
};

/**
 * @brief four peaks of RFI with gaussian wings.
 */
template<typename DataType>
class RfiScenario<3, DataType> : public RfiGenerator<DataType> {
        typedef RfiGenerator<DataType> BaseT;
        typedef typename BaseT::FlaggedDataType FlaggedDataType;
    public:
        RfiScenario();
        ~RfiScenario();
        void next(FlaggedDataType& data) override;
        static constexpr char description[] = "RFI: 4 seperated Gaussian spikes in each block";
};

/**
 * @brief 8 peaks of RFi radonly distributed in the time-frequency block.
 */
template<typename DataType>
class RfiScenario<4, DataType> : public RfiGenerator<DataType> {
        typedef RfiGenerator<DataType> BaseT;
        typedef typename BaseT::FlaggedDataType FlaggedDataType;
    public:
        RfiScenario();
        ~RfiScenario();
        void next(FlaggedDataType& data) override;
        static constexpr char description[] = "RFI: 8 random Gaussian spikes in each block";
};

/**
 * @brief a single saturated channel.
 */
template<typename DataType>
class RfiScenario<5, DataType> : public RfiGenerator<DataType> {
        typedef RfiGenerator<DataType> BaseT;
        typedef typename BaseT::FlaggedDataType FlaggedDataType;
    public:
        RfiScenario();
        ~RfiScenario();
        void next(FlaggedDataType& data) override;
        static constexpr char description[] = "RFI: single saturated channel";
};

#ifdef NDEBUG
/**
 * @brief conseqtive channels with periodic RFI. Ideally we would like our mitigation algorithm to detect these features as they
 * mimic pulsar signals in a narrow band.
 */
template<typename DataType>
class RfiScenario<6, DataType> : public RfiGenerator<DataType>
{
        typedef RfiGenerator<DataType> BaseT;
        typedef typename BaseT::FlaggedDataType FlaggedDataType;

    public:
        RfiScenario();
        ~RfiScenario();
        void next(FlaggedDataType& data) override;
        static constexpr char description[] = "RFI: consecutive channels with variable RFI";
};

/**
 * @brief Strong broadband spike at the center of the block. This is the baseline test for any RFI mitigation algorithm.
 * We will expect for the mitigation algoritm to detect these kind of RFI.
 */
template<typename DataType>
class RfiScenario<7, DataType> : public RfiGenerator<DataType>
{
        typedef RfiGenerator<DataType> BaseT;
        typedef typename BaseT::FlaggedDataType FlaggedDataType;

    public:
        RfiScenario();
        ~RfiScenario();
        void next(FlaggedDataType& data) override;
        static constexpr char description[] = "RFI: broadband spike at the center of the block";
};

/**
 * @brief consecutive channels with weak periodic RFI. Idea is to simulate weak periodic signals in a narrow band.
 * These are not washed out with dedispersion and potentially will be detected as pulsar candidate with the
 * periodicity search algorithms
 */
template<typename DataType>
class RfiScenario<8, DataType> : public RfiGenerator<DataType>
{
        typedef RfiGenerator<DataType> BaseT;
        typedef typename BaseT::FlaggedDataType FlaggedDataType;

    public:
        RfiScenario();
        ~RfiScenario();
        void next(FlaggedDataType& data) override;
        static constexpr char description[] = "RFI: consecutive channels with weak periodic RFI";
};

/**
 * @brief Weak broadband spike at the center of the block. A single weak spike may not effect much for the periodicity
 * search. But a disperse energy may effect SPS efficiency to detect weak bursts. Ideally we would like out mitigation
 * algorithm to find these weak features.
 */
template<typename DataType>
class RfiScenario<9, DataType> : public RfiGenerator<DataType>
{
        typedef RfiGenerator<DataType> BaseT;
        typedef typename BaseT::FlaggedDataType FlaggedDataType;

    public:
        RfiScenario();
        ~RfiScenario();
        void next(FlaggedDataType& data) override;
        static constexpr char description[] = "RFI: Weak (7-sigma) broadband spike at the center of the block.";
};

/**
 * @brief broadband periodic RFI. Idea is to simulate strong periodic signals across the whole band.
 * and periodicity search algorithms will detect these as candidates. In addition, these RFI will mask
 * any genuine candidates in the data and should be mitigated pre search.
 */
template<typename DataType>
class RfiScenario<10, DataType> : public RfiGenerator<DataType>
{
        typedef RfiGenerator<DataType> BaseT;
        typedef typename BaseT::FlaggedDataType FlaggedDataType;

    public:
        RfiScenario();
        ~RfiScenario();
        void next(FlaggedDataType& data) override;
        static constexpr char description[] = "RFI: strong Broadband periodic RFI";
};

#endif // NDEBUG

} // namespace generators


} // namespace cheetah
} // namespace ska
#include "detail/RfiScenario.cpp"

#endif // SKA_CHEETAH_GENERATORS_RFISCENARIO_H
