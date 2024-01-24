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
    }

    void CommandBufferPool::allocateCommandBuffer(CommandBuffer*& commandBuffer)
    {
        VkCommandBuffer vk_command_buffer;

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = primaryCommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        vkAllocateCommandBuffers(VK_G(VkDevice), &allocInfo, &vk_command_buffer);

        commandBuffer = new CommandBuffer(vk_command_buffer);
    }

    void CommandBufferPool::destoryCommandBuffer(CommandBuffer*& commandBuffer)
    {
        VkCommandBuffer& vk_command_buffer = *commandBuffer;
        vkFreeCommandBuffers(VK_G(VkDevice), primaryCommandPool, 1, &vk_command_buffer);

        delete commandBuffer;
    }

}
