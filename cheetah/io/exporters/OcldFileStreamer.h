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
#ifndef SKA_CHEETAH_IO_EXPORTERS_OCLDFILESTREAMER_H
#define SKA_CHEETAH_IO_EXPORTERS_OCLDFILESTREAMER_H

#include "OcldFileStreamerConfig.h"
#include "detail/OcldFileStreamerTraits.h"
#include "cheetah/data/Ocld.h"
#include "panda/OutputFileStreamer.h"

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief
 *    Adapter for Ocld data output to write to timestamped files
 *
 * @tparam NumericalT   Data type stored in the OptimisedCandidate MultiArray data cube
 * @details
 */

template<typename NumericalT>
class OcldFileStreamer final : public panda::OutputFileStreamer<data::Ocld<NumericalT>, OcldFileStreamerTraits<NumericalT>>
{
        typedef panda::OutputFileStreamer<data::Ocld<NumericalT>, OcldFileStreamerTraits<NumericalT>> BaseT;

    public:
        OcldFileStreamer(OcldFileStreamerConfig const&);
        ~OcldFileStreamer();

};

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska

#include "cheetah/io/exporters/detail/OcldFileStreamer.cpp"

#endif // SKA_CHEETAH_IO_EXPORTERS_OCLDFILESTREAMER_H
