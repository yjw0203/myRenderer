#pragma once
#include "rhi/vulkan/yjw_vulkan_global.h"

namespace vulkan
{
    class CommandBuffer
    {
    public:
        CommandBuffer(VkCommandBuffer& in_commandBuffer) : commandBuffer(in_commandBuffer) {}
        operator VkCommandBuffer&() { return commandBuffer; }
    private:
        VkCommandBuffer commandBuffer{};
    };

    class CommandBufferPool
    {
    public:
        void initialize();
        void allocateCommandBuffer(CommandBuffer*& commandBuffer);
        void destoryCommandBuffer(CommandBuffer*& commandBuffer);
    private:
        VkCommandPool primaryCommandPool;
    };

    EXTERN_GLOBAL_REF(CommandBufferPool);
}