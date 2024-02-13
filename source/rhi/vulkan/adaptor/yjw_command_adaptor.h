#pragma once
#include "rhi/new_rhi/yjw_rhi_resource.h"
#include "rhi/vulkan/command/yjw_vulkan_command_buffer.h"
#include "rhi/new_rhi/yjw_rhi_command.h"
namespace vulkan
{
    class RHICommandBufferCreationAdaptor
    {
    public:
        RHICommandBufferCreationAdaptor(const rhi::RHICommandBufferCreation& creation) : payload(creation) {}
        operator VulkanCommandBufferCreation();
    private:
        const rhi::RHICommandBufferCreation& payload;
    };
}