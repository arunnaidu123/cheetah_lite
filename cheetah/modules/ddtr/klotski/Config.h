/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2023 The SKA organisation
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
#ifndef SKA_CHEETAH_MODULES_DDTR_KLOTSKI_CONFIG_H
#define SKA_CHEETAH_MODULES_DDTR_KLOTSKI_CONFIG_H

#include "cheetah/utils/Config.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski {

/**
 * @brief config for klotski DDTR algorithm
 */

class Config : public utils::Config
{
    public:
        Config();
        ~Config();

        /**
         * @brief return true if this algorithm is to be used
         */
        bool active() const;

        /**
         * @brief set the true to use this algorithm
         */
        void active(bool);

        /**
         * @brief return max number of channels per @ref klotski_algorithm_guide "klotski"
         */
        std::size_t max_channels_per_klotski() const;

        /**
         * @brief set max number of channels per @ref klotski_algorithm_guide "klotski"
         */
        void max_channels_per_klotski(unsigned int value);

        /**
         * @brief return max number of channels per @ref klotski_algorithm_guide "klotski"
         */
        std::size_t max_channels_per_band() const;

        /**
         * @brief set max number of channels per @ref klotski_algorithm_guide "klotski"
         */
        void max_channels_per_band(unsigned int value);

        /**
         * @brief return cache size (L1 cache size of the CPU ) to be used per @ref gpu_bruteforce_algorithm_guide "klotski"
         */
        std::size_t cache_size() const;

        /**
         * @brief set cache size (L1 cache size of the CPU set to 1MB by default) to be used per
         * @ref gpu_bruteforce_algorithm_guide "klotski"
         */
        void cache_size(unsigned int value);

        /**
         * @brief returns _precise
         */
        bool precise() const;

        /**
         * @brief set true if the DDTR is computed without any approximation
         */
        void precise(bool);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        bool _active;
        std::size_t _cache_size;
        std::size_t _max_channels_per_klotski;
        std::size_t _max_channels_per_band;
        bool _precise;
};


} // namespace klotski
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_DDTR_KLOTSKI_CONFIG_H
