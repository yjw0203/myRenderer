#include "Private/vulkan/yjw_vulkan_command_queue.h"

namespace rhi
{
    VulkanCommandQueue::VulkanCommandQueue(VulkanDevice* device, QueueFamilyIndices queueFamilyIndices)
        :VulkanDeviceObject(device)
    {
        vkGetDeviceQueue(device->GetNativeDevice(), queueFamilyIndices.graphicsFamily.value(), 0, &m_graphics_queue);
        vkGetDeviceQueue(device->GetNativeDevice(), queueFamilyIndices.presentFamily.value(), 0, &m_present_queue);
    }

    VkQueue VulkanCommandQueue::GetGraphicsQueue()
    {
        return m_graphics_queue;
    }

    VkQueue VulkanCommandQueue::GetPresentQueue()
    {
        return m_present_queue;
    }
}