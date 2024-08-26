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
#include "cheetah/pipelines/search_pipeline/EmptyConfig.h"


namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {


EmptyConfig::EmptyConfig()
    : cheetah::utils::Config("empty")
    , _exit_count(0)
{
}

EmptyConfig::~EmptyConfig()
{
}

void EmptyConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("exit_count", boost::program_options::value<std::size_t>(&_exit_count)->default_value(0), "exit when the pipeline has been called this many times (0=never)");
}

std::size_t EmptyConfig::exit_after() const
{
    return _exit_count;
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
