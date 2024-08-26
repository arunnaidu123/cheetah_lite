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
#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_SAMPLE_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_SAMPLE_H
#include <utility>


namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt {

/**
 * @brief
 *     THe incoming RF signal sample from the UDP stream
 *
 * @details
 *
 */

class Sample
{
    public:
        Sample();
        Sample(short, short, short, short);
        short xx() const;
        short yy() const;
        /// the real and imaginary components of the xy term
        std::pair<short, short> xy() const;

    private:
        short _xx;
        short _yy;
        short _xy_re;
        short _xy_im;
};


} // namespace rcpt
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_SAMPLE_H
