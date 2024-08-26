/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 The SKA organisation
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
#ifndef SKA_CHEETAH_MODULES_RFIM_IQRMCPU_CONFIG_H
#define SKA_CHEETAH_MODULES_RFIM_IQRMCPU_CONFIG_H

#include "cheetah/utils/Config.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace iqrmcpu {

/**
 * @brief
 * @details
 */
class Config : public utils::Config
{
    public:
        Config();
        ~Config();

        /**
         * @brief indicate if the algorithm is to be used
         */
        bool active() const;

        /**
         * @brief Set whether the algorithm is to be used
         */
        void active(bool b);

        /**
         * @brief radius parameter of IQRM, in number of frequency channels
         */
        std::size_t radius() const;

        /**
         * @brief Set the radius radius parameter of IQRM, in number of frequency channels
         * @details When called, the IQRM implementation will enforce radius <= nchan
         */
        void radius(std::size_t r);

        /**
         * @brief threshold parameter of IQRM, in number of Gaussian sigmas
         */
        float threshold() const;

        /**
         * @brief Set the threshold parameter of IQRM, in number of Gaussian sigmas
         */
        void threshold(float t);

        /**
         * @brief number of channels to forcibly flag on both band edges
         */
        std::size_t edge_channels() const;

        /**
         * @brief Set the number of channels to forcibly flag on both band edges
         * @details When called, the IQRM implementation will enforce edge_channels <= nchan
         */
        void edge_channels(std::size_t ec);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        bool _active;
        std::size_t _radius;
        float _threshold;
        std::size_t _edge_channels;
};


} // namespace iqrmcpu
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_RFIM_IQRMCPU_CONFIG_H
