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
#include "cheetah/pipelines/search_pipeline/AccelerationSearchAlgoConfig.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

AccelerationSearchAlgoConfig::AccelerationSearchAlgoConfig(PoolManagerType& pool_manager)
    : BaseT("acceleration")
    , _fdas_config(pool_manager)
    , _tdas_config(pool_manager)
{
    // setup acceleration search options
    add(_fdas_config);
    add(_tdas_config);
}

AccelerationSearchAlgoConfig::~AccelerationSearchAlgoConfig()
{
}

modules::fdas::ConfigType& AccelerationSearchAlgoConfig::fdas_config()
{
    return _fdas_config;
}

modules::fdas::ConfigType const& AccelerationSearchAlgoConfig::fdas_config() const
{
    return _fdas_config;
}

modules::tdas::ConfigType& AccelerationSearchAlgoConfig::tdas_config()
{
    return _tdas_config;
}

modules::tdas::ConfigType const& AccelerationSearchAlgoConfig::tdas_config() const
{
    return _tdas_config;
}

void AccelerationSearchAlgoConfig::add_options(OptionsDescriptionEasyInit&)
{
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
