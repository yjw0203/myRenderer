#pragma once
#include "Public/RHI/rhi/yjw_rhi_define.h"
#include "Private/vulkan/yjw_vulkan_device.h"

namespace rhi
{
    class VulkanTexture : public RHITexture, VulkanDeviceObject
    {
    public:
        VulkanTexture(VulkanDevice* device, const RHITextureDescriptor& desc);
        VulkanTexture(VulkanDevice* device, const RHITextureDescriptor& desc, VkImage existImage, VkImageLayout currentLayout);
        ~VulkanTexture();
        VkImage GetVkImage();
        VkDeviceMemory GetVkDeviceMemory();
        int GetWidth();
        int GetHeight();
        virtual void Update(void* data, int sizeOfByte, int arrayLayer = 0, int mipLevel = 0) override;
        void TransitionState(VkCommandBuffer commandBuffer, VkImageLayout newLayout);// to be multi thread
        void TransitionState(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout);
    private:
        VkImage m_image;
        VkDeviceMemory m_memory;
        VkImageLayout m_current_layout;
        bool m_b_create_from_exist_image = false;
    };

    class VulkanTextureView : public RHITextureView, VulkanDeviceObject
    {
    public:
        VulkanTextureView(VulkanDevice* device, const RHITextureViewDescriptor& desc);
        ~VulkanTextureView();
        VulkanTexture* GetTexture();
        VkImageView GetVkImageView();
        VkFormat GetVkFormat();
        int GetWidth();
        int GetHeight();
    private:
        VkImageView m_view;
    };
}