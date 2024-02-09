#pragma once
#include "rhi/vulkan/yjw_vulkan_global.h"
#include "rhi/vulkan/yjw_vulkan_resource_ruler.h"

namespace vulkan
{
    class VulkanCommandBufferCreation
    {};

    class VulkanCommandBuffer
    {
    public:
        static const VulkanResourceType TypeId = VulkanResourceType::commandBuffer;
        typedef VulkanCommandBufferCreation Creation;
        VulkanCommandBuffer(VkCommandBuffer& in_commandBuffer) : commandBuffer(in_commandBuffer) {}
        operator VkCommandBuffer&() { return commandBuffer; }
        VkCommandBuffer commandBuffer{};
    };

    typedef ResourceHandle<VulkanCommandBuffer> VulkanCommandBufferHandle;

    struct DefaultVulkanCommandBufferAllocateStrategy
    {
        VulkanCommandBuffer* CreateFunc(const VulkanCommandBufferCreation& creation);
        void DestoryFunc(VulkanCommandBuffer* resource);
        VkCommandPool commandPool;
    };

    class CommandBufferPool
    {
    public:
        void initialize();
        VulkanCommandBufferHandle allocateCommandBuffer(VulkanCommandBufferCreation creation);
        void destoryCommandBuffer(VulkanCommandBufferHandle commandBuffer);
    private:
        VkCommandPool primaryCommandPool;
        ResourceAllocator<VulkanCommandBuffer, DefaultVulkanCommandBufferAllocateStrategy> DefaultAllocator;
    };

    EXTERN_GLOBAL_REF(CommandBufferPool);
}