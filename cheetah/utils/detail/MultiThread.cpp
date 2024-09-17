namespace ska {
namespace cheetah {
namespace utils {

template<typename Function, typename... Args>
void MultiThread::add_job(unsigned const& affinity, Function&& fn, Args... args)
{
    _threads.push_back(std::thread([this, affinity, fn, args...](unsigned njobs) {
                set_core_affinity(affinity);
                while(status(njobs))
                {
                    wait(njobs);
                    fn(args...);
                    reset(njobs);
                }
            }, this->_njobs));
    _ready.push_back(false);
    _njobs++;
}


} // namespace utils
} // namespace cheetah
} // namespace ska