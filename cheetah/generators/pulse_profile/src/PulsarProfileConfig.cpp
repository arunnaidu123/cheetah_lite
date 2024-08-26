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
#include "../PulsarProfileConfig.h"


namespace ska {
namespace cheetah {
namespace generators {


PulsarProfileConfig::PulsarProfileConfig(std::string const& name)
    : utils::Config(name)
    , _db_id("B0011+47")
{
}

PulsarProfileConfig::~PulsarProfileConfig()
{
}

void PulsarProfileConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("profile_id", boost::program_options::value<std::string>()->notifier( [this](std::string const& val) { _db_id = PulsarProfileId(val); }), "ebta profile identifier (http://www.epta.eu.org/epndb/) default=B0011+47");
}

PulsarProfileId const& PulsarProfileConfig::profile_id() const
{
    return _db_id;
}

} // namespace generators
} // namespace cheetah
} // namespace ska
