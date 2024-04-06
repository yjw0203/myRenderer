#pragma once
#include "RHI/rhi/yjw_rhi_define.h"
#include "yjw_vulkan_device.h"
#include "yjw_vulkan_resource_cast.h"

namespace rhi
{
    class VulkanBuffer : public RHIBuffer, VulkanDeviceObject
    {
    public:
        VulkanBuffer(VulkanDevice* device, const RHIBufferDescriptor& desc);
        ~VulkanBuffer();
        VkBuffer GetVkBuffer();
        VkDeviceMemory GetVkDeviceMemory();
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
    private:
        VkBufferView m_view;
    };
}