#pragma once
#include "RHI/rhi/yjw_rhi_device.h"
#include "RHI/rhi/yjw_rhi_object.h"
#include "RHI/rhi/yjw_rhi_layer.h"

namespace rhi
{
    class RHIInstanceImpl : public RHIObject
    {
    public:
        RHIInstanceImpl(const RHIInstanceConfig& config);
        virtual ~RHIInstanceImpl();
        virtual RHIDevice* CreateDevice() = 0;
        const RHIInstanceConfig& GetConfig();
        RHILayer* GetRHILayer(RHILayerType type);

        void OnInstanceInit(RHIInstanceImpl* instance);
        void OnDeviceInit(RHIDevice* device);
        void OnDeviceShutdown(RHIDevice* device);
        void OnSwapchainInit(class RHISwapChain* swapchain);
        void OnSwapchainShutdown(class RHISwapChain* swapchain);
    private:
        RHIInstanceConfig m_config{};
        RHILayer* m_rhi_layers[rhi_layer_count] = {};
    };

    class RHIInstance 
    {
    public:
        RHIInstance(RHIInstanceConfig config);
        ~RHIInstance();
        RHIDevice* CreateDevice();
        RHILayer* GetRHILayer(RHILayerType type);
    private:
        RHIInstanceImpl* m_impl = nullptr;
    };
}