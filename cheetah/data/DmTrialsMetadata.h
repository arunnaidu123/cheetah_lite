#ifndef SKA_CHEETAH_DATA_DMTRIALSMETADATA_H
#define SKA_CHEETAH_DATA_DMTRIALSMETADATA_H

#include "cheetah/data/Units.h"
#include "cheetah/data/DedispersionMeasure.h"
#include "cheetah/data/DefaultAllocator.h"
#include "cheetah/data/Series.h"
#include "panda/DataChunk.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/functional/hash.hpp>
#pragma GCC diagnostic pop


namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief     Container for metadata that describes a set of dispersion measure trials.
 *
 * @details   The DmTrialsMetadata class is used to store information required to instantiate
 *            a DmTrials object. The emplace_back method is used to register a new DmTrial into
 *            the instance. Upon registering, the instance will update a hash value that encodes
 *            the order and number of DM values in the DmTrialsMetadata object. This hash value is
 *            used to check whether two DmTrialsMetadata objects are compatible (check via == operator.)
 *
 *            To generate a DmTrials object using a DmTrialsMetadata object we do the following:
 *
 * @code      //Create a new shared pointer to a DmTrialsMetadata object
 *            auto metadata = DmTrialsMetadata::make_shared(sampling_interval,fundamental_sample_count);
 *
 *            //Populate with some random DM values
 *            for (std::size_t dm_idx=0; dm_idx<ndms; ++dm_idx)
 *            {
 *                metadata->emplace_back(DmTrialsMetadata::DmType(dm_idx * data::parsecs_per_cube_cm), 1);
 *            }
 *
 *            //Generate DmTrials object
 *            auto trials = DmTrialsType::make_shared(metadata,epoch);
 * @endcode
 */

class DmTrialsMetadata
    : public panda::DataChunk<DmTrialsMetadata>
{
    public:
        typedef DedispersionMeasureType<float> DmType;
        typedef SecondsType<double> TimeType;

    public:
        /**
         * @brief      Container for an metadata concerning a single DM trial.
         */
        class Metadata
        {
            public:
                /**
                 * @brief      Create a new Metadata instance
                 *
                 * @param      parent               The parent DmTrialsMetadata instance
                 * @param[in]  dm                   The dispersion measure
                 * @param[in]  downsampling_factor  The downsampling factor
                 */
                Metadata(DmTrialsMetadata const& parent, DmType dm, std::size_t downsampling_factor=1);
                ~Metadata();

                /**
                 * @brief      return the dispersion measure
                 */
                DmType const& dm() const;

                /**
                 * @brief      return the sampling interval
                 */
                TimeType sampling_interval() const;

                /**
                 * @brief      return the size (number of samples)
                 */
                std::size_t size() const;

                /**
                 * @brief      return the size (number of samples)
                 */
                std::size_t downsampling_factor() const;

            private:
                DmTrialsMetadata const& _parent;
                DmType _dm;
                std::size_t _downsampling_factor;
        };

    private:
        typedef std::vector<Metadata> ContainerType;

    public:
        /**
         * @brief      Create a new DmTrialsMetadata instance
         *
         * @param[in]  fundamental_sampling_interval  The fundamental sampling interval
         * @param[in]  fundamental_sample_count       The fundamental sample count
         *
         * @details     The fundamental_sampling_interval and fundamental_sample_count are important,
         *             as all Metadata instances contained in this DmTrialsMetadata instances will have
         *             sampling intervals that are integer multiples of fundamental_sampling_interval and
         *             lengths that will be zero-remainder integer divisiors of fundamental_sample_count.
         */
        DmTrialsMetadata(TimeType fundamental_sampling_interval, std::size_t fundamental_sample_count);
        ~DmTrialsMetadata();

        /**
         * @brief      Add a new Metadata instance containing information about a single DM trial
         *
         * @param[in]  dm                   The dispersion measure of the trial
         * @param[in]  downsampling_factor  The downsampling factor of the trial
         */
        void emplace_back(DmType dm, std::size_t downsampling_factor = 1);

        /**
         * @brief      Return a const reference to a Metadata instance
         *
         * @param[in]  n     The index of the instance to return
         */
        ContainerType::const_reference operator[](std::size_t n) const;

        /**
         * @brief      Return a reference to a Metadata instance
         *
         * @param[in]  n     The index of the instance to return
         */
        ContainerType::reference operator[](std::size_t n);

        /**
         * @brief      Return an iterator pointing to the first Metadata instance contained
         */
        ContainerType::iterator begin();

        /**
         * @brief      Return an iterator pointing to the first Metadata instance contained
         */
        ContainerType::const_iterator begin() const;

        /**
         * @brief      Return an const iterator pointing to the first Metadata instance contained
         */
        ContainerType::const_iterator cbegin() const;

        /**
         * @brief      Return an iterator pointing beyond the Metadata instance contained
         */
        ContainerType::iterator end();

        /**
         * @brief      Return an iterator pointing beyond the Metadata instance contained
         */
        ContainerType::const_iterator end() const;

        /**
         * @brief      Return a const iterator pointing beyond the Metadata instance contained
         */
        ContainerType::const_iterator cend() const;

        /**
         * @brief      Return the number of Metadata instances contained
         */
        std::size_t size() const;

        /**
         * @brief      Test if this instance is compatible with another instance
         *
         * @param      other  The other instance against which to compare
         *
         * @return     true if compatible, false otherwise
         */
        bool operator==(DmTrialsMetadata const& other);

        /**
         * @brief      Return the temporal duration of the trials
         *
         * @detailed   This is simply fundamental_sampling_interval * fundamental_sample_count
         */
        TimeType duration() const;

        /**
         * @brief      Return the fundamental_sampling_interval
         */
        TimeType fundamental_sampling_interval() const;

        /**
         * @brief      Return the total size of the data represented
         */
        std::size_t total_data_size() const;

        /**
         * @brief      Return the number of samples
         */
        std::size_t number_of_samples() const;

        std::vector<unsigned> number_of_ranges() const;

    private:
        TimeType _sampling_interval;
        std::size_t _number_of_samples;
        std::size_t _hash_value;
        ContainerType _metadata;
};


} // namespace data
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_DATA_DMTRIALSMETADATA_H
