#pragma once
#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"
#include <optional>

namespace rhi
{
    class VulkanCommandQueue : public VulkanDeviceObject
    {
    public:
        VulkanCommandQueue(VulkanDevice* device, QueueFamilyIndices queueFamilyIndices);
        VkQueue GetGraphicsQueue();
        VkQueue GetPresentQueue();
    private:
        VkQueue m_graphics_queue = nullptr;
        VkQueue m_present_queue = nullptr;
    };
}