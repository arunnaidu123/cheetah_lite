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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_BEAMCONFIG_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_BEAMCONFIG_H

#include "cheetah/pipelines/search_pipeline/DataConfig.h"
#include "cheetah/data/DataSrcConfig.h"
#include "cheetah/sigproc/Config.h"
#include "cheetah/psrdada/Config.h"
#include "cheetah/io/producers/rcpt/SkaUdpConfig.h"
#include "panda/ConfigModule.h"
#include "panda/Thread.h"
#include "panda/ThreadConfig.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

/**
 * @brief
 *    Parse configuration parameters for a single beam in the pipeline instance of cheetah
 *
 */

template<typename NumericalT>
class BeamConfig : public utils::Config
{
        typedef utils::Config BaseT;
        typedef DataConfig<NumericalT> DataConfigType;

    public:
        BeamConfig(std::string const& tag_name = "beam");
        ~BeamConfig();

        /**
         * @brief return the thread config
         */
        panda::ThreadConfig const& thread_config() const;

        /**
         * @brief return the sigproc config
         */
        sigproc::Config const& sigproc_config() const;

        /**
         * @brief return the psrdada config
         */
        psrdada::Config const& psrdada_config() const;

        /**
         * @brief return the rcpt config associated with the Udp Stream
         */
        template<io::producers::rcpt::SkaSelector::Stream Stream>
        auto udp_rcpt_config() const -> typename io::producers::rcpt::SkaUdpStream<Stream>::Config const&;

        /**
         * @brief return the rcpt_mid_config
         */
        io::producers::rcpt_mid::Config const& rcpt_mid_config() const;

        /**
         * @brief return the rcpt_low_config
         */
        io::producers::rcpt_low::Config const& rcpt_low_config() const;

        /**
         * @brief set the thread config
         */
        void thread_config(panda::ThreadConfig const& thread_config);

        /**
         * @brief return the data export specific configuration parameters
         */
        DataConfigType const& data_config() const;

        /**
         * @brief set the enabled status of the beam
         */
        void active(bool status);

        /**
         * @brief sget the enabled status of the beam
         */
        bool active() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        bool _active;
        panda::ThreadConfig _thread_config;
        sigproc::Config _sigproc_config;
        psrdada::Config _psrdada_config;
        data::DataSrcConfig _data_src_config;
        io::producers::rcpt::SkaUdpConfig _rcpt_config;
        DataConfigType _data_config;
};

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "detail/BeamConfig.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_BEAMCONFIG_H
