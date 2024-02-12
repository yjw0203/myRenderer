#pragma once
#include <map>
#include "rhi/rpi/yjw_rpi_header.h"
namespace yjw
{
    using namespace rpi;
    class Pass
    {
    public:
        virtual void buildPSO() = 0;
        virtual void setupData() = 0;
        virtual void recordCommand(RPICommandBuffer commandBuffer) = 0;
        virtual void submit() = 0;
        virtual void setResourceBarrier();
    protected:
        std::map<RPIResource, RPIResourceState> initialResourceState;
    };
}