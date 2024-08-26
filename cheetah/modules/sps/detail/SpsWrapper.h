/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#ifndef SKA_CHEETAH_MODULES_SPS_SPSWRAPPER_H
#define SKA_CHEETAH_MODULES_SPS_SPSWRAPPER_H


namespace ska {
namespace cheetah {
namespace modules {
namespace sps {

/**
 * @brief Wrapper fot sps algos to manage the handler that is calles with the single pulse search results
 */

template<class WrappedT, typename SpsTraits>
class SpsWrapper : public WrappedT
{
        typedef WrappedT BaseT;
        typedef typename SpsTraits::SpHandler SpHandler;
        typedef typename SpsTraits::DedispersionHandler DmHandler;
        typedef typename SpsTraits::BufferType BufferType;
        typedef typename SpsTraits::DmTrialsType DmTrialsType;

    public: // typedefs
        typedef typename WrappedT::Architecture Architecture;
        typedef typename WrappedT::Config Config;

    public:
        /**
         * @brief contructor for SpsWrapper
         * @details passes the config to BaseT which is template parameter Wrapped.
         * @param config Sps config
         * @param sp_handler reference to the Sps hanler which takes in SPS output
         */
        SpsWrapper(sps::Config const& config, SpHandler& sp_handler);

        /**
         * @brief call the wrapped algorithm and call the SpHandler with the result
         */
        std::shared_ptr<DmTrialsType> operator()(panda::PoolResource<Architecture>&, BufferType);

    private:
        SpHandler& _sp_handler;
};


} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "SpsWrapper.cpp"

#endif // SKA_CHEETAH_MODULES_SPS_SPSWRAPPER_H
