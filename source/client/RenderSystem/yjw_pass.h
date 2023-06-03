#pragma once
#include <map>
#include "rhi/rhi/yjw_rhi_header.h"
namespace yjw
{
    using namespace rhi;
    class Pass
    {
    public:
        virtual void buildPSO() = 0;
        virtual void setupData() = 0;
        virtual void recordCommand() = 0;
        virtual void submit() = 0;
        virtual void setResourceBarrier();
    protected:
        std::map<std::shared_ptr<RHIResource>, rhi::RHIResourceState> initialResourceState;
    };
}