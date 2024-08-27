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

#include <numeric>
namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {
namespace cpu {

template<typename SpdtTraits>
MsdEstimator<SpdtTraits>::MsdEstimator(DmTrialsType const& data)
{
	double sum = std::accumulate(data[0].begin(), data[0].end(), 0.0);
	double mean = sum / data[0].size();

	std::vector<double> diff(data[0].size());
	std::transform(data[0].begin(), data[0].end(), diff.begin(), [mean](double x) { return x - mean; });

	double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
	_stdev = std::sqrt(sq_sum / data[0].size());
    _mean = mean;
}

template<typename SpdtTraits>
MsdEstimator<SpdtTraits>::~MsdEstimator()
{
}

template<typename SpdtTraits>
double MsdEstimator<SpdtTraits>::mean()
{
	return _mean;
}

template<typename SpdtTraits>
double MsdEstimator<SpdtTraits>::stdev()
{
	return _stdev;
}

} // namespace cpu
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska