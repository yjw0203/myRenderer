#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_define.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"

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
        int GetLayerCount();
        int GetMipCount();
        virtual void Update(void* data, int sizeOfByte, int arrayLayer = 0, int mipLevel = 0) override;
        virtual void MapForReadback(int arrayLayer, int mipLevel, void*& data, int& byte_per_pixel, int& byte_per_raw) override;
        virtual void UnMapReadback() override;
        void TransitionToOriginState(VkCommandBuffer commandBuffer);
        void TransitionState(VkCommandBuffer commandBuffer, VkImageLayout newLayout);// to be multi thread
        void TransitionState(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout);
    private:
        VkImage m_image;
        VkDeviceMemory m_memory;
        VkImageLayout m_origin_layout{};
        VkImageLayout m_current_layout{};
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