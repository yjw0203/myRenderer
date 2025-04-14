#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_render_pass.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"

namespace rhi
{

    class VulkanRenderPass : public RHIRenderPass, VulkanDeviceObject
    {
    public:
        VulkanRenderPass(VulkanDevice* device, const RHIRenderPassDescriptor &desc);
        ~VulkanRenderPass();
        VkRenderPass GetNativeRenderPass();
        VkFramebuffer GetFramebuffer();
        virtual int GetWidth() override;
        virtual int GetHeight() override;
        void TransitionStateToWrite(VkCommandBuffer commandBuffer);
        void TransitionStateToRead(VkCommandBuffer commandBuffer);
    private:
        VkRenderPass m_native_render_pass = nullptr;
        VkFramebuffer m_frame_buffer = nullptr;
        VkImageView m_image_view[RHI_MAX_RENDER_TARGETS] = {};
        int m_width = 0;
        int m_height = 0;
    };
}