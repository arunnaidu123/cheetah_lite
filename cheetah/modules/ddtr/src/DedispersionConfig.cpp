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
#include "cheetah/modules/ddtr/DedispersionConfig.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {


DedispersionConfig::DedispersionConfig()
    : utils::Config("dedispersion")
    , _dm_start(0.0 * data::parsecs_per_cube_cm)
    , _dm_end(100.0 * data::parsecs_per_cube_cm)
    , _dm_step(10.0 * data::parsecs_per_cube_cm)
{
}

DedispersionConfig::DedispersionConfig(Dm start, Dm end, Dm step)
    : utils::Config("dedispersion")
    , _dm_start(start)
    , _dm_end(end)
    , _dm_step(step)
{
}

DedispersionConfig::~DedispersionConfig()
{
}

void DedispersionConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("start", boost::program_options::value<float>()->notifier(
        [&](float f)
        {
            _dm_start = f * data::parsecs_per_cube_cm;
        }), "start DM in cm^-3 pc"
    )
    ("end", boost::program_options::value<float>()->notifier(
        [&](float f)
        {
            _dm_end = f * data::parsecs_per_cube_cm;
        }), "end DM in cm^-3 pc (inclusive)"
    )
    ("step", boost::program_options::value<float>()->notifier(
        [&](float f)
        {
            _dm_step = f * data::parsecs_per_cube_cm;
        })
        , "DM step size in cm^-3 pc");
}

DedispersionConfig::Dm DedispersionConfig::dm_start() const
{
    return _dm_start;
}

DedispersionConfig::Dm DedispersionConfig::dm_end() const
{
    return _dm_end;
}

DedispersionConfig::Dm DedispersionConfig::dm_step() const
{
    return _dm_step;
}

void DedispersionConfig::dm_start(Dm dm)
{
    _dm_start = dm;
}

void DedispersionConfig::dm_end(Dm dm)
{
    _dm_end = dm;
}

void DedispersionConfig::dm_step(Dm dm)
{
    _dm_step = dm;
}

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
