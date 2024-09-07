#include "cheetah/utils/MultiThread.h"
#include "panda/Log.h"
#include "panda/Error.h"

namespace ska {
namespace cheetah {
namespace utils {

MultiThread::MultiThread()
{
    _terminate = false;
    _njobs =0;
}

MultiThread::~MultiThread()
{
    std::cout<<"~MultiThread()\n";
}

void MultiThread::reset(unsigned id)
{
    std::lock_guard lk(_m);
    _ready[id] = false;
    _cv.notify_all();
}


void MultiThread::ready(unsigned id)
{
    std::lock_guard lk(_m);
    _ready[id] = true;
    _cv.notify_all();
}

void MultiThread::wait(unsigned id)
{
    std::unique_lock lk(_m);
    _cv.wait(lk, [id, this]{ return _ready[id]; });
}

void MultiThread::finish(unsigned id)
{
    std::unique_lock lk(_m);
    _cv.wait(lk, [id, this]{ return !_ready[id]; });
}

bool MultiThread::status(unsigned id)
{
    std::unique_lock lk(_mt);
    return !_terminate;
}

void MultiThread::terminate()
{
    std::unique_lock lk(_mt);
    _terminate = true;
}

unsigned MultiThread::number_of_jobs() const
{
    return _njobs;
}

#if HAS_PTHREAD_SET_AFFINITY
void MultiThread::set_core_affinity(unsigned const& affinity)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(affinity, &cpuset);

    int rc = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    if (rc != 0) throw panda::Error("Thread: Error calling pthread_setaffinity_np: ");
}
#else

void MultiThread::set_core_affinity(unsigned const&)
{
    // do nothing if no pthread
    PANDA_LOG_WARN << "unable to set thread affinity - recompile with pthread library to activate this functionality";
}

#endif

} // namespace utils
} // namespace cheetah
} // namespace ska