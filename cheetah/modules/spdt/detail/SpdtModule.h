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
#ifndef SKA_CHEETAH_MODULES_SPDT_SPDTMODULE_H
#define SKA_CHEETAH_MODULES_SPDT_SPDTMODULE_H

#include "cheetah/modules/spdt/cpu/Spdt.h"
#include "cheetah/modules/spdt/klotski/Spdt.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {

/**
 * @brief An spdt module configurable with a number of arguments
 */

template<typename SpdtTraits, template<typename> class... SpdtAlgorithms>
class SpdtModule
{
        typedef typename SpdtTraits::SpHandler SpHandler;
        typedef typename SpdtTraits::Config Config;
        typedef typename SpdtTraits::DmTrialsType DmTrialsType;

        typedef panda::ConfigurableTask<typename SpdtTraits::Pool
                                      , SpHandler const&
                                      , shared_ptr<DmTrialsType> const&
                                      > TaskType;

    public:
        SpdtModule(Config const& config, SpHandler const& sp_handler);

        void operator()(shared_ptr<DmTrialsType> const& data);

    private:
        SpHandler _sp_handler;
        TaskType _task;
};

} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/spdt/detail/SpdtModule.cpp"

#endif // SKA_CHEETAH_MODULES_SPDT_SPDTMODULE_H
