#include "cheetah/modules/rfim/sum_threshold/Config.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace sum_threshold {


Config::Config()
    : cheetah::utils::Config("rfim_sum_threshold")
    , _active(false)
    , _threshold_sizes{1, 2, 4, 6, 8, 16, 32, 64}
{
}

Config::~Config()
{
}

bool Config::active() const
{
    return _active;
}

void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("active", boost::program_options::value<bool>(&_active)->default_value(false), "use this algorithm for rfi clipping")
    ("its_cutoff", boost::program_options::value<float>(&_its_cutoff_threshold)->default_value(6.0), "median cutoff threshold")
    ("base_sensitivity", boost::program_options::value<float>(&_base_sensitivity)->default_value(1.0), "the sensitivity of the threshold to std deviation")
    ("window", boost::program_options::value<unsigned>(&_window)->default_value(0U), "restrict area of data sum thresholding takes place (0=all data)");
}

std::vector<unsigned> const& Config::thresholding_data_sizes() const
{
    return _threshold_sizes;
}

float Config::its_cutoff_threshold() const
{
    return _its_cutoff_threshold;
}

unsigned Config::window() const {
    return _window;
}

float Config::base_sensitivity() const
{
    return _base_sensitivity;
}
} // namespace sum_threshold
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
