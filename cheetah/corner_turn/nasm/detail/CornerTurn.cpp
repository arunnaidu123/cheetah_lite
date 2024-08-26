#include "cheetah/corner_turn/nasm/detail/NasmBatcher.h"
#include "cheetah/corner_turn/cpu/BatchedCpuCornerTurnAlgo.h"

namespace ska {
namespace cheetah {
namespace corner_turn {
namespace nasm {

namespace {

struct BatcherTraits {
    typedef cpu::BatchedAlgoDefaultCpuTraits CpuTraits;
    typedef nasm::NasmBatcher<CpuTraits> BatchOptimiser;
};

} // namespace

template <typename SourceIteratorT, typename DestinIteratorT>
void CornerTurn::copy_corner_turn_random_access(
              SourceIteratorT&& src_it
            , DestinIteratorT&& dst_it
            , std::size_t num_input_innerloop_elements
            , std::size_t num_input_outerloop_elements)
{
    cpu::BatchedCpuCornerTurnAlgo<BatcherTraits>::copy_corner_turn(
              &(*src_it)
            , &(*dst_it)
            , num_input_innerloop_elements
            , num_input_outerloop_elements);
}

template <typename SourceIteratorT, typename DestinIteratorT>
void corner_turn( SourceIteratorT&& src_it
                , DestinIteratorT&& dst_it
                , std::size_t num_input_innerloop_elements
                , std::size_t num_input_outerloop_elements)
{
    CornerTurn::copy_corner_turn(
                            std::forward<SourceIteratorT>(src_it)
                          , std::forward<DestinIteratorT>(dst_it)
                          , num_input_innerloop_elements
                          , num_input_outerloop_elements);
}

} // namespace nasm
} // namespace corner_turn
} // namespace cheetah
} // namespace ska
