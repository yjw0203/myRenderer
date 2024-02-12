#include "yjw_pass.h"
#include "rhi/rpi/yjw_rpi_header.h"
namespace yjw
{
    void Pass::setResourceBarrier()
    {
        for (auto pair : initialResourceState)
        {
            //rhi::IRHI::Get()->resourceBarrier(pair.first.get(), pair.first->state, pair.second);
        }
    }
}