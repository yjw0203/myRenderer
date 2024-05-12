#pragma once
#include "RHI/rhi/yjw_rhi_layer.h"
#include "RHI/rpi/yjw_rpi_define.h"
#include "vulkan/vulkan.h"

namespace rhi
{
    class RHIImguiLayer : public RHILayer
    {
    public:
        RHIImguiLayer(ERHIType rhiType);
        virtual void OnInstanceInit(class RHIInstanceImpl* instance);
        virtual void OnDeviceInit(class RHIDevice* device);
        virtual void OnDeviceShutdown(class RHIDevice* device);
        virtual void OnSwapchainInit(class RHISwapChain* swapchain);
        virtual void OnSwapchainShutdown(class RHISwapChain* swapchain);

        void NewFrame(RHIContext* context, RHIRenderPass* renderPass);
        void Render(class RHIContext* context);

    private:
        VkDescriptorPool m_descriptor_pool{};
        RHITexture* m_texture = nullptr;
        RHITextureView* m_texture_view = nullptr;
        rpi::RPIRenderPass m_render_pass{};

    };
}