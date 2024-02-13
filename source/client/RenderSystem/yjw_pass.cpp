#include "yjw_pass.h"
#include "rhi/rpi/yjw_rpi_header.h"
namespace yjw
{
    void Pass::setResourceBarrier(RPICommandBuffer commandBuffer)
    {
        for (auto pair : initialResourceState)
        {
            rpi::RPICmdResourceBarrier(commandBuffer, pair.first, rpi::RPIGetResourceState(pair.first), pair.second);
        }
    }
}