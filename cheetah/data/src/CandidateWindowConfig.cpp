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
#include "cheetah/data/CandidateWindowConfig.h"


namespace ska {
namespace cheetah {
namespace data {


CandidateWindowConfig::CandidateWindowConfig()
    : BaseT("candidate_window"),
      _window(0.0 * boost::units::si::milli * boost::units::si::seconds, 0.0 * boost::units::si::milli * boost::units::si::seconds)
{
}

CandidateWindowConfig::~CandidateWindowConfig()
{
}

void CandidateWindowConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
      ("ms_before", boost::program_options::value<double>()->default_value(static_cast<double>(_window.ms_before().value()))->notifier([this](double v){
                        _window.ms_before(v * milliseconds);
                    })
                    , "the number of milliseconds of data to incclude before the start of the candidate")
      ("ms_after", boost::program_options::value<double>()->default_value(static_cast<double>(_window.ms_after().value()))->notifier([this](double v) {
                        _window.ms_after(v * milliseconds);
                    })
                    , "the number of milliseconds of data to incclude before the end of the candidate");
}


CandidateWindow const& CandidateWindowConfig::window() const
{
    return _window;
}

CandidateWindow& CandidateWindowConfig::window()
{
    return _window;
}


void CandidateWindowConfig::window( CandidateWindow& window)
{
    window = _window;
}

CandidateWindowConfig& CandidateWindowConfig::operator=( const CandidateWindowConfig& window_config )
{
    _window.ms_before(window_config.window().ms_before());
    _window.ms_after( window_config.window().ms_after());
    return *this;
}

} // namespace data
} // namespace cheetah
} // namespace ska
