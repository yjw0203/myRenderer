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
    }

    VulkanCommandBuffer* DefaultVulkanCommandBufferAllocateStrategy::CreateFunc(const VulkanCommandBufferCreation& creation)
    {
        VkCommandBuffer vk_command_buffer;

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        vkAllocateCommandBuffers(VK_G(VkDevice), &allocInfo, &vk_command_buffer);

        VulkanCommandBuffer* commandBuffer = new VulkanCommandBuffer(vk_command_buffer);
        return commandBuffer;
    }

    void DefaultVulkanCommandBufferAllocateStrategy::DestoryFunc(VulkanCommandBuffer* commandBuffer)
    {
        VkCommandBuffer& vk_command_buffer = *commandBuffer;
        vkFreeCommandBuffers(VK_G(VkDevice), commandPool, 1, &vk_command_buffer);
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

}
