/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 The SKA organisation
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
#ifndef SKA_CHEETAH_MODULES_SPDT_KLOTSKI_COMMON_CONFIG_H
#define SKA_CHEETAH_MODULES_SPDT_KLOTSKI_COMMON_CONFIG_H


#include "cheetah/utils/Config.h"
#include <panda/CommaSeparatedOptions.h>

namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {
namespace klotski_common {

/**
 * @brief
 *   Configuration for the Spdt klotski_common
 */

class Config : public utils::Config
{
    public:
        Config(std::string module_name);
        ~Config();

        /**
         * @brief array containing widths to be searched
         * @details widths should in samples
         */
        std::vector<unsigned> const& widths() const;
        void widths(std::vector<unsigned> const& widths);

        /**
         * @brief maximum width limit
         * @details This should be a multiple of 1024. This width need not be in the widths array but it is used to
         * estimate the overlap.
         */
        unsigned int const& max_width_limit() const;
        void max_width_limit(unsigned int& value);

        /**
         * @brief batch size of the width search. The DM are batched in number of _number_of_dms_per_iteration.
         * @details The safe number is 8. TODO: check if we can go to 16 with newer harware.
         */
        unsigned int const& number_of_dms_per_iteration() const;
        void number_of_dms_per_iteration(unsigned int& value);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        panda::CommaSeparatedOptions<unsigned> _widths;
        unsigned int _number_of_dms_per_iteration;
        unsigned int _max_width_limit;
};


} // namespace klotski_common
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_SPDT_KLOTSKI_COMMON_CONFIG_H