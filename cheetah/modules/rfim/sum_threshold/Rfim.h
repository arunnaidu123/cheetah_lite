#ifndef SKA_CHEETAH_MDULES_RFIM_SUM_THRESHOLD_RFIM_H
#define SKA_CHEETAH_MDULES_RFIM_SUM_THRESHOLD_RFIM_H

#include "cheetah/modules/rfim/sum_threshold/Config.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/modules/rfim/RfimBase.h"
#include "cheetah/data/RfimFlaggedData.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace sum_threshold {

/**
 * @brief
 *    A CPU implementation of the sum_threshold algorithm
 *
 * @details
 *
 */

template<typename RfimTraits>
class Rfim : public RfimBase<Rfim<RfimTraits>, typename RfimTraits::Policy>
{
        typedef RfimBase<Rfim<RfimTraits>, typename RfimTraits::Policy> BaseT;
        friend BaseT;

        typedef typename RfimTraits::Policy Policy;
        typedef typename PolicyInfo<Policy>::AdapterType DataAdapter;

    public:
        typedef typename PolicyInfo<Policy>::ReturnType ReturnType;
        typedef cheetah::Cpu Architecture;

    public:
        Rfim(Config const& config);
        ~Rfim();

        using BaseT::operator();

    protected:
        template<typename DataType>
        void operator()(DataType& data, DataAdapter& adpater);

    private:
        Config const& _config;
};


} // namespace sum_threshold
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "detail/Rfim.cpp"

#endif // SKA_CHEETAH_MDULES_RFIM_SUM_THRESHOLD_RFIM_H