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
#ifndef SKA_CHEETAH_PRODUCERS_SPCCLSPEADPRODUCER_H
#define SKA_CHEETAH_PRODUCERS_SPCCLSPEADPRODUCER_H


#ifdef ENABLE_SPEAD
#include "cheetah/io/exporters/SpCclSpeadReader.h"
#include "cheetah/data/SpCandidateData.h"
#include "panda/Producer.h"

namespace ska {
namespace cheetah {
namespace producers {

/**
 * @brief An SpCclReader adapter for use as a Producer for the DataManager
 * @details
 */

template<typename DerivedType, typename TimeFrequencyT>
class SpCclSpeadProducerCRTP : public panda::Producer<DerivedType, data::SpCandidateData<TimeFrequencyT>>
{
        typename panda::Producer<DerivedType, data::SpCandidateData<TimeFrequencyT>> BaseT;

    private:
        typedef TimeFrequencyT TimeFrequencyType;
        //typedef data::SpCandidateData<TimeFrequencyType> SpCclType;

    public:
        explicit SpCclSpeadProducerCRTP(io::exporters::SpCclSpeadReaderConfig const&);
        ~SpCclSpeadProducerCRTP();
        SpCclSpeadProducerCRTP(SpCclSpeadProducerCRTP const&) = delete;
        SpCclSpeadProducerCRTP(SpCclSpeadProducerCRTP&&) = delete;
        typedef data::SpCandidateData<TimeFrequencyType> SpCclType;

        void init();
        bool process();

    private:
        struct NullCallback;
        class DataFactory {
            public:
                DataFactory(SpCclSpeadProducerCRTP& producer)
                    : _producer(producer)
                {}

                std::shared_ptr<SpCclType> operator()();

            private:
                SpCclSpeadProducerCRTP& _producer;
        };

    private:
        DataFactory _data_factory;
        io::exporters::SpCclSpeadReader<TimeFrequencyType, NullCallback, DataFactory>  _reader;
};

template<typename TimeFrequencyT>
class SpCclSpeadProducer : public SpCclSpeadProducerCRTP<SpCclSpeadProducer<TimeFrequencyT>, TimeFrequencyT>
{
        typedef SpCclSpeadProducerCRTP<SpCclSpeadProducer<TimeFrequencyT>, TimeFrequencyT> BaseT;

    public:
        SpCclSpeadProducer(io::exporters::SpCclSpeadReaderConfig const&);
        using SpCclSpeadProducerCRTP<SpCclSpeadProducer<TimeFrequencyT>, TimeFrequencyT>::SpCclSpeadProducerCRTP;
};

} // namespace producers
} // namespace cheetah
} // namespace ska
#include "detail/SpCclSpeadProducer.cpp"

#endif // ENABLE_SPEAD
#endif // SKA_CHEETAH_PRODUCERS_SPCCLSPEADPRODUCER_H
