#include "cheetah/pipelines/candidate_pipeline/Launcher.h"
#include <cstdint>

int main(int argc, char** argv)
{
    ska::cheetah::pipelines::candidate_pipeline::Launcher<uint8_t> launcher;
    if(int rv=launcher.parse(argc, argv)) return rv;
    return launcher.exec();
}
