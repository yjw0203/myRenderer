#pragma once
#include "RHI/rhi/yjw_rhi_define.h"

namespace rhi
{
    class RHILayer
    {
    public:
        RHILayer(ERHIType rhiType) : m_rhi_type(rhiType) {};
        ERHIType GetRHIType() { return m_rhi_type; }

        virtual void OnInstanceInit(class RHIInstanceImpl* instance) {};
        virtual void OnDeviceInit(class RHIDevice* device) {};
        virtual void OnDeviceShutdown(class RHIDevice* device) {};
        virtual void OnSwapchainInit(class RHISwapChain* swapchain) {};
        virtual void OnSwapchainShutdown(class RHISwapChain* swapchain) {};
    private:
        ERHIType m_rhi_type;
    };
}