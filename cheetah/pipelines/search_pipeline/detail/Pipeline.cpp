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
#include "cheetah/pipelines/search_pipeline/Pipeline.h"
#include "cheetah/pipelines/search_pipeline/PipelineHandler.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

template<typename InputDataStream, typename PipelineHandlerType>
std::shared_ptr<panda::Pipeline<InputDataStream>> create_pipeline(InputDataStream& stream, PipelineHandlerType& fn )
{
    //return std::make_shared<Pipeline<InputDataStream>>(stream, config, std::bind(&PipelineHandler::operator(), &fn, std::placeholders::_1));
    return std::make_shared<panda::Pipeline<InputDataStream>>(stream, std::bind(&PipelineHandlerType::operator(), &fn, std::placeholders::_1));
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
