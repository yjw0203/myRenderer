#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_layer.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_define.h"
#include "vulkan/vulkan.h"

namespace rhi
{
    struct VkTextureId
    {
        VkDescriptorPool m_descriptor_pool{};
        VkDescriptorSetLayout m_descriptor_set_layout{};
        VkDescriptorSet m_texture_id{};
        RHITextureView* m_texture_view{};
    };

    class RHIImguiLayer : public RHILayer
    {
    public:
        RHIImguiLayer(ERHIType rhiType);
        virtual void OnInstanceInit(class RHIInstanceImpl* instance);
        void Init(class RHIDevice* device);
        virtual void OnDeviceShutdown(class RHIDevice* device);
        virtual void OnSwapchainInit(class RHISwapChain* swapchain);
        virtual void OnSwapchainShutdown(class RHISwapChain* swapchain);

        void NewFrame();
        void Render(class RHIContext* context);

        void* RegisterTexture(const char* name, RHITextureView* texture);
        void* GetImTextureID(const char* name);

    private:
        VkDescriptorPool m_descriptor_pool{};
        RHITexture* m_texture = nullptr;
        RHITextureView* m_texture_view = nullptr;
        rpi::RPIShader m_vs{};
        rpi::RPIShader m_ps{};
        rpi::RPIRenderPass m_render_pass{};
        rpi::RPIRenderPipeline m_render_pipeline{};
        std::unordered_map<std::string, VkTextureId> m_registered_textures;


        RHIDevice* m_device = nullptr;

    };
}