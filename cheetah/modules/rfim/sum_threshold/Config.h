#ifndef SKA_CHEETAH_MODULES_RFIM_SUM_THRESHOLD_CONFIG_H
#define SKA_CHEETAH_MODULES_RFIM_SUM_THRESHOLD_CONFIG_H


#include "cheetah/utils/Config.h"
#include <vector>

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace sum_threshold {

/**
 * @brief
 *
 * @details
 *
 */

class Config : public cheetah::utils::Config
{
    public:
        Config();
        ~Config();

        bool active() const;

        unsigned window() const;

        std::vector<unsigned> const& thresholding_data_sizes() const;

        float base_sensitivity() const;
        float its_cutoff_threshold() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options);

    private:
        bool _active;
        unsigned _window;
        float _its_cutoff_threshold;
        float _base_sensitivity;
        std::vector<unsigned> _threshold_sizes;
};


} // namespace sum_threshold
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_RFIM_SUM_THRESHOLD_CONFIG_H