#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_define.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_resource_cast.h"

namespace rhi
{
    class VulkanBuffer : public RHIBuffer, VulkanDeviceObject
    {
    public:
        VulkanBuffer(VulkanDevice* device, const RHIBufferDescriptor& desc);
        ~VulkanBuffer();
        VkBuffer GetVkBuffer();
        VkDeviceMemory GetVkDeviceMemory();

        virtual void Update(const void* data, int bufferOffset, int sizeOfByte) override;
    private:
        VkBuffer m_buffer;
        VkDeviceMemory m_memory;
    };

    class VulkanBufferView : public RHIBufferView, VulkanDeviceObject
    {
    public:
        VulkanBufferView(VulkanDevice* device, const RHIBufferViewDescriptor& desc);
        ~VulkanBufferView();
        VulkanBuffer* GetBuffer();
        int GetOffset();
        VkBufferView& GetView();
    private:
        VkBufferView m_view{};
    };
}