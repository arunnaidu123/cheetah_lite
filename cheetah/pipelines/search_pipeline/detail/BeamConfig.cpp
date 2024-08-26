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
#include "cheetah/pipelines/search_pipeline/BeamConfig.h"
#include <iostream>

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

template<typename NumericalT>
BeamConfig<NumericalT>::BeamConfig(std::string const& tag)
    : BaseT(tag)
    , _active(true)
    , _rcpt_config(_data_src_config)
{
    // setup data source options
    add(_thread_config);
    add(_data_src_config);
    add(_data_config);

    _data_src_config.add(_sigproc_config);
    _data_src_config.add(_psrdada_config);
}

template<typename NumericalT>
BeamConfig<NumericalT>::~BeamConfig()
{
}

template<typename NumericalT>
typename BeamConfig<NumericalT>::DataConfigType const& BeamConfig<NumericalT>::data_config() const
{
    return _data_config;
}

template<typename NumericalT>
sigproc::Config const& BeamConfig<NumericalT>::sigproc_config() const
{
    return _sigproc_config;
}

template<typename NumericalT>
psrdada::Config const& BeamConfig<NumericalT>::psrdada_config() const
{
    return _psrdada_config;
}

template<typename NumericalT>
template<io::producers::rcpt::SkaSelector::Stream Stream>
auto BeamConfig<NumericalT>::udp_rcpt_config() const -> typename io::producers::rcpt::SkaUdpStream<Stream>::Config const&
{
    return _rcpt_config.template rcpt_config<Stream>();
}

template<typename NumericalT>
io::producers::rcpt_mid::Config const& BeamConfig<NumericalT>::rcpt_mid_config() const
{
    return _rcpt_config.template rcpt_config<io::producers::rcpt::SkaSelector::Stream::SkaMid>();
}

template<typename NumericalT>
io::producers::rcpt_low::Config const& BeamConfig<NumericalT>::rcpt_low_config() const
{
    return _rcpt_config.template rcpt_config<io::producers::rcpt::SkaSelector::Stream::SkaLow>();
}

template<typename NumericalT>
panda::ThreadConfig const& BeamConfig<NumericalT>::thread_config() const
{
    return _thread_config;
}

template<typename NumericalT>
void BeamConfig<NumericalT>::thread_config(panda::ThreadConfig const& thread_config)
{
    _thread_config = thread_config;
}

template<typename NumericalT>
void BeamConfig<NumericalT>::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("active", boost::program_options::bool_switch()->default_value(_active)->notifier([&](bool val) { _active = val; }), "enable this beam");
}

template<typename NumericalT>
bool BeamConfig<NumericalT>::active() const
{
    return _active;
}

template<typename NumericalT>
void BeamConfig<NumericalT>::active(bool active)
{
    _active = active;
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
