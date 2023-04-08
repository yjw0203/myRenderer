#pragma once

#include "vulkan/vulkan.h"
#include <vector>
#include <queue>
namespace rhi
{
    class VulkanCommandBufferAllocater
    {
    public:
        static VulkanCommandBufferAllocater& Get()
        {
            static VulkanCommandBufferAllocater instance;
            return instance;
        }
        void initialize();
        void allocate(VkCommandBuffer& commandBuffer);
        void free(VkCommandBuffer& commandBuffer);

        VkCommandBuffer beginImmdiatelyCommandBuffer();
        void endImmdiatelyCommandBuffer(VkCommandBuffer& commandBuffer);

        VkCommandBuffer beginOneTimeCommandBuffer();
        void endOneTimeCommandBuffer(VkCommandBuffer& commandBuffer);
        void resetOneTimeCommandBuffer();
        
    private:
        VkCommandPool primaryCommandPool;

        VkCommandPool oneTimeCommandBufferPool;
        int currentUseOneTimeCommandBufferIndex = 0;
        std::vector<VkCommandBuffer> oneTimeCommandBuffers;
    };
}