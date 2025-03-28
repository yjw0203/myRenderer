#pragma once
#include <map>
#include "Engine/rhi/Public/rpi/yjw_rpi_header.h"
namespace yjw
{
    using namespace rpi;
    class Pass
    {
    public:
        virtual void buildPSO() = 0;
        virtual void setupData() = 0;
        virtual void recordCommand(RPIContext commandBuffer) = 0;
        virtual void submit() = 0;
    };
}