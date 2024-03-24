#pragma once
#include "yjw_rhi_object.h"

namespace rhi
{
    class RHISwapChain : public RHIObject
    {
    public:
        virtual void Prensent(bool bSync) = 0;
    };
}