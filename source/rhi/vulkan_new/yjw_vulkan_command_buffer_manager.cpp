#include "yjw_vulkan_command_buffer_manager.h"
#include "yjw_vulkan_god_objects_manager.h"
namespace rhi
{
    void VulkanCommandBufferAllocater::initialize()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = vulkanGod.queueFamilyIndices.graphicsFamily.value();
        vkCreateCommandPool(vulkanGod.device, &poolInfo, nullptr, &primaryCommandPool);

        vkCreateCommandPool(vulkanGod.device, &poolInfo, nullptr, &oneTimeCommandBufferPool);

        allocate(currentCommandBuffer);
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(currentCommandBuffer, &beginInfo);
    }

    void VulkanCommandBufferAllocater::allocate(VkCommandBuffer& commandBuffer)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = primaryCommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        vkAllocateCommandBuffers(vulkanGod.device, &allocInfo, &commandBuffer);
    }

    void VulkanCommandBufferAllocater::free(VkCommandBuffer& commandBuffer)
    {
        vkFreeCommandBuffers(vulkanGod.device, primaryCommandPool, 1, &commandBuffer);
    }

    VkCommandBuffer VulkanCommandBufferAllocater::beginImmdiatelyCommandBuffer()
    {
        VkCommandBuffer commandBuffer;
        if (currentUseOneTimeCommandBufferIndex < oneTimeCommandBuffers.size())
        {
            commandBuffer = oneTimeCommandBuffers[currentUseOneTimeCommandBufferIndex++];
        }
        else
        {
            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = oneTimeCommandBufferPool;
            allocInfo.commandBufferCount = 1;
            vkAllocateCommandBuffers(vulkanGod.device, &allocInfo, &commandBuffer);
            oneTimeCommandBuffers.push_back(commandBuffer);
            currentUseOneTimeCommandBufferIndex++;
        }

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void VulkanCommandBufferAllocater::endImmdiatelyCommandBuffer(VkCommandBuffer& commandBuffer)
    {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(vulkanGod.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(vulkanGod.graphicsQueue);
    }

    VkCommandBuffer& VulkanCommandBufferAllocater::getCurrentCommandBuffer()
    {
        return currentCommandBuffer;
    }

    void VulkanCommandBufferAllocater::reset()
    {
        vkResetCommandPool(vulkanGod.device, oneTimeCommandBufferPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
        currentUseOneTimeCommandBufferIndex = 0;
        
        vkResetCommandBuffer(currentCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(currentCommandBuffer, &beginInfo);
    }

}