#pragma once
#include "rhi/vulkan/yjw_vulkan_global.h"
#include "rhi/vulkan/yjw_vulkan_resource_ruler.h"
#include <array>
namespace vulkan
{
    class VulkanCommandBufferCreation
    {};

    class VulkanCommandBuffer
    {
    public:
        static const VulkanResourceType TypeId = VulkanResourceType::commandBuffer;
        typedef VulkanCommandBufferCreation Creation;
        VulkanCommandBuffer() {}
        operator VkCommandBuffer&() { return commandBuffer; }
        VkCommandBuffer commandBuffer{};
        VkFence fence;
    };

    typedef ResourceHandle<VulkanCommandBuffer> VulkanCommandBufferHandle;

    struct DefaultVulkanCommandBufferAllocateStrategy
    {
        VulkanCommandBuffer* CreateFunc(const VulkanCommandBufferCreation& creation);
        void DestoryFunc(VulkanCommandBuffer* resource);
        VkCommandPool commandPool;
    };

    struct OneTimeCommandBuffer
    {
        VkCommandBuffer commandBuffer;
        int index;
    };

    class CommandBufferPool
    {
    public:
        void initialize();
        VulkanCommandBufferHandle allocateCommandBuffer(VulkanCommandBufferCreation creation);
        void destoryCommandBuffer(VulkanCommandBufferHandle commandBuffer);
        OneTimeCommandBuffer beginImmdiatelyCommandBuffer();
        void endImmdiatelyCommandBuffer(OneTimeCommandBuffer& commandBuffer);
    private:
        VkCommandPool primaryCommandPool;
        ResourceAllocator<VulkanCommandBuffer, DefaultVulkanCommandBufferAllocateStrategy> DefaultAllocator;

        VkCommandPool oneTimeCommandBufferPool;
        int currentUseOneTimeCommandBufferIndex = 0;
        static const int oneTimeCommandBuffersCount = 100;
        std::array<VkCommandBuffer, oneTimeCommandBuffersCount> oneTimeCommandBuffers;
        std::array<VkFence, oneTimeCommandBuffersCount> oneTimeCommandBufferFences;
    };

    EXTERN_GLOBAL_REF(CommandBufferPool);
}