#ifndef SKA_CHEETAH_DATA_DMTRIALS_H
#define SKA_CHEETAH_DATA_DMTRIALS_H

#include "cheetah/data/DmTrial.h"
#include "cheetah/data/DmTrialsMetadata.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/DedispersionMeasure.h"
#include "cheetah/data/DefaultAllocator.h"
#include "cheetah/data/Series.h"
#include "cheetah/utils/ModifiedJulianClock.h"
#include "panda/DataChunk.h"
#include <vector>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief   A continuous memory container for dispersion measure trials of varying downsamplings.
 *
 * @details This is a continuous memory container for storing dispersion measure trials. The
 *          continuous nature of the underlying memory is a design details rather than an implementation
 *          detail. The continuous memory is required for two reasons:
 *
 *          1. Most dedispersion algorithms output their results in a continuous memory block
 *          2. In order to get high memory throughput on copies to and from accelerator devices it
 *             is necessary to have large amounts of memory being copied. This is usually not achievable
 *             when copying each dispersion measure trial individually.
 *
 *          A DmTrials object is instantiated with a shared pointer to a a DmTrialsMetadata object. This object
 *          contains information such as the DM, downsampling value and length of each DM trial to be stored
 *          in the DmTrials object.
 *
 *          The size of a DmTrials object is fixed at instantiation and cannot be changed afterwards. This
 *          means that DMs cannot be added or removed. To access the dispersion measure trial time series
 *          (DmTrial objects) contained within the DmTrials object one may either index the DmTrials object
 *          or iterator over it. An example of this is given below:
 *
 * @code    //Create a new DmTrials object as a shared pointer
 *          auto dm_trials = DmTrials::make_shared(<DmTrialsMetadata shared pointer>, <MJD start of the first sample in each trial>)
 *
 *          //To access the nth trial we use the operator[] overload:
 *          auto trial = (*dm_trials)[n]
 *
 *          //To further access the data in the trial, we simply iterate over it
 *          for (auto const& x: trial)
 *          {
 *              ...do something with x...
 *          }
 * @endcode
 *
 *          A DmTrials object provides two important methods that allow the object to be used in building
 *          data processing pipelines, these are is_compatible and is_contiguous. The DmTrialsMetadata instance
 *          used to create the DmTrials instance will have a hash value associated with it.
 *          For DmTrials objects with the same DMs in the same order and with the same lengths and downsamplings,
 *          this hash value will be the same. The is_compatible method compares the value of this hash between two
 *          DmTrials instances and returns true if the hashes are the same. The is_contiguous method allows for the
 *          user to check if two DmTrials instances are contiguous in time.
 *
 */
template <typename Arch, typename NumericT, typename Alloc=typename DefaultAllocator<Arch,NumericT>::type>
class DmTrials
    : public panda::DataChunk<DmTrials<Arch,NumericT,Alloc>>
{
        template<typename, typename, typename> friend class DmTrials;

    public:
        typedef DedispersionMeasureType<float> DmType;
        typedef Series<Arch,NumericT,Alloc> SeriesType;
        typedef DmTrial<SeriesType> DmTrialType;
        typedef typename DmTrialType::TimeType TimeType;
        typedef cheetah::utils::ModifiedJulianClock::time_point Mjd;
        typedef typename std::vector<DmTrialType>::iterator Iterator;
        typedef typename std::vector<DmTrialType>::const_iterator ConstIterator;
        typedef NumericT ValueType;

        typedef Arch Architecture;
        typedef Alloc Allocator;

    public:

        /**
          * Construct a defined size DmTrials object
          */
        DmTrials(std::shared_ptr<DmTrialsMetadata> metadata, Mjd start, Alloc const& allocator=Alloc());

        /**
          * Construct a copy of the DmTrials data on another device
          */
        template<typename OtherArch, typename OtherNumericT, typename OtherAlloc>
        DmTrials(DmTrials<OtherArch, OtherNumericT, OtherAlloc> const&, Alloc const& allocator=Alloc());

        /**
         * @brief empty object constructor
         */
        ~DmTrials();

        /**
         * @returns the absolute time the first time sample corresponds to
         */
        Mjd const& start_time() const;

        /**
         * @brief      Set the absolute start time
         *
         * @param      start_time  The start time
         */
        void start_time(Mjd start_time);

        /**
         * @brief      Return reference to DmTrial instance
         *
         * @param[in]  n     The index of the DmTrial instance
         */
        DmTrialType& operator[](std::size_t n);
        DmTrialType const& operator[](std::size_t n) const;

        /**
         * @brief      Return an iterator pointing to the start of the DM trials container
         */
        Iterator begin();
        ConstIterator begin() const;
        ConstIterator cbegin() const;

        /**
         * @brief      Return an iterator pointing to the end of the DM trials container
         */
        Iterator end();
        ConstIterator end() const;
        ConstIterator cend() const;

        /**
         * @brief      Return the number of DmTrials
         */
        std::size_t size() const;

        /**
         * @brief      Determines if another DmTrials object is compatible with this one.
         *
         * @details     Compatibility is determined by checking that both DmTrials objects have
         *             equivalent metadada. This implies that they have the same DM trial values
         *             with the same downsampling ratio in the same order.
         *
         * @param      other  The other DmTrials object
         *
         * @return     True if compatible, False otherwise.
         */
        bool is_compatible(DmTrials const& other) const;

        /**
         * @brief      Determines if another DmTrials object is contiguous in time after this one.
         *
         * @param      other  The other DmTrials object
         *
         * @return     True if contiguous, False otherwise.
         */
        bool is_contiguous(DmTrials const& other) const;

        /**
         * @brief      Return the time duration of the DmTrials
         *
         * @return     A time duration
         */
        TimeType duration() const;

        /**
         * @brief      Return the trials meta data object
         */
        DmTrialsMetadata const& metadata() const;

        void swap(DmTrials& b);

    private:
        std::shared_ptr<DmTrialsMetadata> _metadata;
        SeriesType _data;
        std::vector<DmTrialType> _trials;
        Mjd _start_time;
        TimeType _duration;
};


} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/DmTrials.cpp"

#endif // SKA_CHEETAH_DATA_DMTRIALS_H
