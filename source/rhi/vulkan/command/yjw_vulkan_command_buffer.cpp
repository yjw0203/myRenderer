#include "yjw_vulkan_command_buffer.h"

namespace vulkan
{
    void CommandBufferPool::initialize()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = VK_G(QueueFamilyIndices).graphicsFamily.value();
        vkCreateCommandPool(VK_G(VkDevice), &poolInfo, nullptr, &primaryCommandPool);
        DefaultAllocator.allocateStrategy.commandPool = primaryCommandPool;

        vkCreateCommandPool(VK_G(VkDevice), &poolInfo, nullptr, &oneTimeCommandBufferPool);
        for (int i = 0; i < oneTimeCommandBuffersCount; i++)
        {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = oneTimeCommandBufferPool;
            allocInfo.commandBufferCount = 1;
            vkAllocateCommandBuffers(VK_G(VkDevice), &allocInfo, &oneTimeCommandBuffers[i]);

            VkFenceCreateInfo fenceInfo{};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            vkCreateFence(VK_G(VkDevice), &fenceInfo, nullptr, &oneTimeCommandBufferFences[i]);
        }
    }

    VulkanCommandBuffer* DefaultVulkanCommandBufferAllocateStrategy::CreateFunc(const VulkanCommandBufferCreation& creation)
    {
        VulkanCommandBuffer* commandBuffer = new VulkanCommandBuffer();

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        vkAllocateCommandBuffers(VK_G(VkDevice), &allocInfo, &commandBuffer->commandBuffer);

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        vkCreateFence(VK_G(VkDevice), &fenceInfo, nullptr, &commandBuffer->fence);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(commandBuffer->commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void DefaultVulkanCommandBufferAllocateStrategy::DestoryFunc(VulkanCommandBuffer* commandBuffer)
    {
        vkFreeCommandBuffers(VK_G(VkDevice), commandPool, 1, &commandBuffer->commandBuffer);
        vkDestroyFence(VK_G(VkDevice), commandBuffer->fence, nullptr);
        delete commandBuffer;
    }

    VulkanCommandBufferHandle CommandBufferPool::allocateCommandBuffer(VulkanCommandBufferCreation creation)
    {
        return DefaultAllocator.create(creation);
    }

    void CommandBufferPool::destoryCommandBuffer(VulkanCommandBufferHandle commandBuffer)
    {
        DefaultAllocator.destory(commandBuffer);
    }

    VkCommandBuffer CommandBufferPool::beginImmdiatelyCommandBuffer()
    {
        VkCommandBuffer commandBuffer;
        vkWaitForFences(VK_G(VkDevice), 1, &oneTimeCommandBufferFences[currentUseOneTimeCommandBufferIndex], VK_TRUE, UINT64_MAX);
        commandBuffer = oneTimeCommandBuffers[currentUseOneTimeCommandBufferIndex];
        vkResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        currentUseOneTimeCommandBufferIndex = (currentUseOneTimeCommandBufferIndex + 1) % oneTimeCommandBuffersCount;
        return commandBuffer;
    }

    void CommandBufferPool::endImmdiatelyCommandBuffer(VkCommandBuffer& commandBuffer)
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        int index = (&commandBuffer - oneTimeCommandBuffers.data()) / sizeof(VkCommandBuffer);
        vkQueueSubmit(VK_G(VkGraphicsQueue), 1, &submitInfo, oneTimeCommandBufferFences[index]);
    }

}
