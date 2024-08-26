#ifndef SKA_CHEETAH_MODULES_RFIM_LASTUNFLAGGED_H
#define SKA_CHEETAH_MODULES_RFIM_LASTUNFLAGGED_H

#include "cheetah/data/TimeFrequency.h"
#include "panda/ModuloNumber.h"
#include <memory>
#include <vector>

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {

/**
 * @brief Policy that replaces any bad specturm with the last unflagged spectrum value
 *
 */
template<typename TimeFrequencyType>
class LastUnflagged;

template<typename TimeFrequencyType>
class LastUnflaggedAdapter {
        static_assert(!std::is_const<TimeFrequencyType>::value, "LastUnflagged must be able to modify the TimeFrequencyBlock yet has been passed a const");
        typedef typename TimeFrequencyType::DataType NumericalRep;
        typedef typename TimeFrequencyType::ConstSpectra Spectra;
        typedef typename TimeFrequencyType::ConstChannel ConstChannel;
        typedef typename TimeFrequencyType::Channel Channel;
        typedef std::shared_ptr<TimeFrequencyType> ReturnType;

    public:
        LastUnflaggedAdapter(LastUnflagged<TimeFrequencyType>& parent, TimeFrequencyType& data);
        ~LastUnflaggedAdapter();

        void data();

        template<typename SliceT>
        typename std::enable_if<pss::astrotypes::is_slice<SliceT>::value, void>::type
        mark_good(SliceT const&);
        void mark_good(NumericalRep const&, data::DimensionIndex<data::Frequency> channel_number);

        template<typename SliceType>
        typename std::enable_if<pss::astrotypes::is_slice<SliceType>::value && std::remove_reference<SliceType>::type::rank != 1, void>::type
             mark_bad(SliceType const& slice);

        template<typename SliceType>
        typename std::enable_if<pss::astrotypes::is_slice<SliceType>::value && std::remove_reference<SliceType>::type::rank == 1, void>::type
             mark_bad(SliceType const& slice);

        void mark_bad(Channel const& channel);
        void mark_bad(ConstChannel const& channel);

        /// mark a channel as bad across the whole data set
        void mark_bad(data::DimensionIndex<data::Frequency> channel_number);

        void mark_bad(NumericalRep const&, data::DimensionIndex<data::Frequency> channel_number);

    private:
        LastUnflagged<TimeFrequencyType>& _parent;
        TimeFrequencyType* _data;
        std::vector<NumericalRep>  _replacement_sample;
        NumericalRep const*      _last_good_spectrum;
};

template<typename TimeFrequencyType>
class LastUnflagged
{
        typedef typename TimeFrequencyType::DataType NumericalRep;
        typedef std::vector<NumericalRep> SampleType;
        typedef typename TimeFrequencyType::Spectra Spectrum;
        typedef LastUnflaggedAdapter<TimeFrequencyType> Adapter;
        friend Adapter;

    public:
        LastUnflagged();
        ~LastUnflagged();

        Adapter adapter(TimeFrequencyType& data);

    protected:
        void set_good_sample(Spectrum const&);
        void set_good_sample(NumericalRep const& sample_start, std::size_t size);
        SampleType& last_good_spectrum();

    private:
        std::vector<SampleType> _replacement_sample;
        panda::ModuloNumber<unsigned> _sample_index;
        panda::ModuloNumber<unsigned> _read_index;
};


} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "detail/LastUnflagged.cpp"

#endif // SKA_CHEETAH_MODULES_RFIM_LASTUNFLAGGED_H
