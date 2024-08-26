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
#include "panda/Log.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace sps {


template<class SpsTraits>
SpsAlgoFactory<SpsTraits>::SpsAlgoFactory(ConfigType const& config, typename SpsTraits::SpHandler& sp_handler)
    : _config(config)
    , _sp_handler(sp_handler)
{
}

template<class SpsTraits>
void SpsAlgoFactory<SpsTraits>::none_selected() const
{
    PANDA_LOG_WARN << "No sps algorithm has been selected";
}

template<class SpsTraits>
template<typename T>
T SpsAlgoFactory<SpsTraits>::create() const
{
    return T(_config, _sp_handler);
}

} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska
