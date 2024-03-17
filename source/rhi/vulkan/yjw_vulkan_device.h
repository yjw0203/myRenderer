#pragma once
#include "RHI/rhi/yjw_rhi_define.h"
#include "yjw_vulkan_instance.h"
#include "vulkan/vulkan.h"
#include <optional>

namespace rhi
{
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    class VulkanDevice : public RHIDevice
    {
    public:
        VulkanDevice(VulkanInstance* vkInstance, bool isDebugMode);
        ~VulkanDevice();

        virtual RHISwapChain* CreateSwapchain(void* window) override;

        VkPhysicalDevice GetGpu();
        VkDevice GetNativeDevice();
        VulkanInstance* GetParentInstance();
        QueueFamilyIndices GetQueueFamilyIndices();
        class VulkanCommandQueue* GetCommandQueue();
    private:
        VulkanInstance* m_parent_instance = nullptr;
        VkPhysicalDevice m_gpu = nullptr;
        VkDevice m_native_device = nullptr;
        QueueFamilyIndices m_queue_family_indices{};
        class VulkanCommandQueue* m_command_queue = nullptr;
    };

    class VulkanDeviceObject
    {
    public:
        VulkanDeviceObject(VulkanDevice* device);
        VulkanDevice* GetDevice();
    private:
        VulkanDevice* m_parent_device = nullptr;
    };
}