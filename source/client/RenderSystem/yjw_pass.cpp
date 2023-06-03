#include "yjw_pass.h"
#include "rhi/rhi/yjw_rhi_header.h"
namespace yjw
{
    void Pass::setResourceBarrier()
    {
        for (auto pair : initialResourceState)
        {
            rhi::IRHI::Get()->resourceBarrier(pair.first.get(), pair.first->state, pair.second);
        }
    }
}