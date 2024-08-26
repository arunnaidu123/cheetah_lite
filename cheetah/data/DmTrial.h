#ifndef SKA_CHEETAH_DATA_DMTRIAL_H
#define SKA_CHEETAH_DATA_DMTRIAL_H

#include "cheetah/data/DmTrialsMetadata.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/SeriesSlice.h"
#include "cheetah/data/DedispersionMeasure.h"
#include "cheetah/utils/ModifiedJulianClock.h"

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief    A wrapper class that presents a SeriesSlice object as a trial dispersion
 *           measure time series.
 *
 * @details  This class provides some necessary functionality to allow a user to access
 *           individual dispersion measure trials stored inside a DmTrials object
 *           (a single contiguous array that split into multiple dispersion measure trials).
 *
 *           This class is not intended to be instantiated directly by the user, but instead
 *           instances of this class will be returned to the user by a DmTrials object.
 *
 *           This class adds to pieces of functionality over the basic SeriesSlice object,
 *           namely methods to access the DM and sampling interval of the data contained in
 *           the slice.
 *
 */
template <typename SeriesType>
class DmTrial
    : public SeriesSlice<SeriesType>
{
    public:
        typedef typename SeriesType::Iterator Iterator;
        typedef DedispersionMeasureType<float> DmType;
        typedef boost::units::quantity<Seconds, double> TimeType;

    public:
        /**
         * @brief      Create new instance
         *
         * @param      dm                 The dispersion measure of the trial
         * @param      sampling_interval  The sampling interval
         * @param[in]  start_it           The start iterator
         * @param[in]  end_it             The end iterator
         */
        DmTrial(DmTrialsMetadata::Metadata const& metadata, Iterator start_it, Iterator end_it);
        ~DmTrial();

        /**
         * @brief      Return the sampling interval
         */
        TimeType sampling_interval() const;

        /**
         * @brief      Return the dispersion measure of the trial
         */
        DmType dm() const;

    private:
        DmTrialsMetadata::Metadata const& _metadata;
};


} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/DmTrial.cpp"

#endif // SKA_CHEETAH_DATA_DMTRIAL_H
