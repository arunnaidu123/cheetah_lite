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
#include "cheetah/io/exporters/SpCclSpeadStreamer.h"
#include "panda/Log.h"
#include <spead2/send_udp.h>


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

namespace {

template<typename T, typename Enabled = void>
struct TypeHelper
{};

template<typename T>
struct TypeHelper<T, typename std::enable_if<std::is_integral<T>::value>::type>
{
    static std::pair<char, std::size_t> format() { return std::make_pair('i', sizeof(T)*8);  }
    static std::string numpy() { return "i" + std::to_string(sizeof(T)); }
};

template<typename T>
struct TypeHelper<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
{
    static std::pair<char, std::size_t> format() { return std::make_pair('f', sizeof(T)*8);  }
};

template<typename T>
struct KeepAlive {
    KeepAlive(spead2::flavour& fl, T const& cand_list)
        : heap(fl)
        , _data_ptr(cand_list.shared_from_this())
    {
    }

    public:
        spead2::send::heap heap;
        std::vector<double> tstart_memory;
        std::size_t number_of_channels;
        boost::units::quantity<boost::units::si::time, double> sample_interval;

    private:
        std::shared_ptr<const T> _data_ptr;
};

} // namespace

template<typename NumericalRep>
SpCclSpeadStreamer& SpCclSpeadStreamer::operator<<(data::SpCcl<NumericalRep> const& cand_list)
{
    typedef typename data::SpCcl<NumericalRep>::SpCandidateType CandidateType;

    if(cand_list.empty()) return *this;

    data::CandidateWindowConfig const& window_config = _config.window_config();
    data::CandidateWindow const& window = window_config.window();
    typename data::SpCcl<NumericalRep>::ConstDataIterator data_it = cand_list.data_begin(window);
    typename data::SpCcl<NumericalRep>::ConstDataIterator data_end = cand_list.data_end();

    // descriptors
    _tf_data_desc.format.clear();
    _tf_data_desc.format.push_back(TypeHelper<NumericalRep>::format());

    while(data_it != data_end)
    {
        // Any local variables here will not be in scope when the async call to write a heap
        // is made. So any variables we want to write out we store in the KeepAlive block
        typedef KeepAlive<data::SpCcl<NumericalRep>> KeepAliveType;
        std::shared_ptr<KeepAliveType> keep_alive = std::make_shared<KeepAliveType>(_sp_flavour, cand_list);

        spead2::send::heap& heap = keep_alive->heap;
        std::vector<double>& tstart_memory = keep_alive->tstart_memory;

        heap.add_descriptor(_tf_data_desc);
        heap.add_descriptor(_data_start_time_desc);
        heap.add_descriptor(_data_number_of_channels_desc);
        heap.add_descriptor(_data_channel_1);
        heap.add_descriptor(_data_channel_width);
        heap.add_descriptor(_data_sammple_interval);
        heap.add_descriptor(_candidate_start_time_desc);
        heap.add_descriptor(_candidate_dm_desc);
        heap.add_descriptor(_candidate_width_desc);
        heap.add_descriptor(_candidate_sigma_desc);
        heap.add_descriptor(_candidate_duration_desc);

        // tf meta data
        auto tf_start_it = cand_list.tf_blocks().begin();
        keep_alive->number_of_channels = (*tf_start_it)->number_of_channels();
        keep_alive->sample_interval = (*tf_start_it)->sample_interval();

        // grouped candidates
        auto cand_block_it = data_it.candidate_begin();
        auto const& cand_block_end = data_it.candidate_end();
        std::size_t tstart_size=std::distance(cand_block_it, cand_block_end) + cand_list.tf_blocks().size();
        tstart_memory.reserve(tstart_size);
        while(cand_block_it != cand_block_end) {
            auto const& candidate = &*cand_block_it;
            tstart_memory.emplace_back(cand_list.start_time(*candidate).time_since_epoch().count());
            auto const& start_mjd=tstart_memory.back();
            heap.add_item(candidate_start_time_id, (void*)&(start_mjd), sizeof(start_mjd), false);
            heap.add_item(candidate_dm_id, (void*)&candidate->dm().value(), sizeof(candidate->dm().value()), false);
            heap.add_item(candidate_width_id, (void*)&candidate->width(), sizeof(candidate->width()), false);
            heap.add_item(candidate_sigma_id, (void*)&candidate->sigma(), sizeof(typename CandidateType::SigmaType), false);
            heap.add_item(candidate_duration_id, (void*)&candidate->tend(), sizeof(candidate->tend()), false);
            ++cand_block_it;
        }

        // TF data associated with this candidate block
        bool end_block;
        do {
            heap.add_item(data_number_of_channels_id, &keep_alive->number_of_channels, sizeof(std::size_t), false);
            heap.add_item(data_sample_interval_id, &reinterpret_cast<double const&>(keep_alive->sample_interval), sizeof(double), false);

            // candidate tf data
            tstart_memory.emplace_back(data_it.start_time().time_since_epoch().count());
            auto const& start_time=tstart_memory.back();
            heap.add_item(tf_data_start_time_id, (void*)&start_time, sizeof(decltype(start_time)), false);
            heap.add_item(tf_data_id, (void*)&*(*data_it).begin(), (*data_it).data_size()*sizeof(NumericalRep), false);

            end_block = data_it.end_block();
            ++data_it;
        }
        while(!end_block);
        assert(tstart_memory.size() <= tstart_size);

        // send the heap
        _sp_stream.async_send_heap(heap, [ keep_alive ] (const boost::system::error_code &ec, spead2::item_pointer_t)
                                                        {
                                                            if (ec) {
                                                                PANDA_LOG_ERROR << "spead2 output stream failure: " << ec.message();
                                                            }
                                                        });

    }

    return *this;
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
