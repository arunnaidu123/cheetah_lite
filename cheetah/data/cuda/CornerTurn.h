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

namespace ska {
namespace cheetah {
namespace data {
namespace cuda {

void corner_turn(const uint8_t* d_input, uint8_t* d_output, std::size_t first_dimension, std::size_t second_dimension);
void corner_turn(const uint8_t* d_input, uint16_t* d_output, std::size_t first_dimension, std::size_t second_dimension);
void corner_turn(const uint8_t* d_input, float* d_output, std::size_t first_dimension, std::size_t second_dimension);
void corner_turn(const uint16_t* d_input, uint8_t* d_output, std::size_t first_dimension, std::size_t second_dimension);
void corner_turn(const uint16_t* d_input, uint16_t* d_output, std::size_t first_dimension, std::size_t second_dimension);
void corner_turn(const uint16_t* d_input, float* d_output, std::size_t first_dimension, std::size_t second_dimension);
void corner_turn(const float* d_input, uint8_t* d_output, std::size_t first_dimension, std::size_t second_dimension);
void corner_turn(const float* d_input, uint16_t* d_output, std::size_t first_dimension, std::size_t second_dimension);
void corner_turn(const float* d_input, float* d_output, std::size_t first_dimension, std::size_t second_dimension);

}
}
}
}