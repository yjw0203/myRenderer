#pragma once
#include "yjw_rhi_swap_chain.h"

namespace rhi
{
    class RHIDevice
    {
    public:
        virtual RHISwapChain* CreateSwapchain(void* window) = 0;
    };
}