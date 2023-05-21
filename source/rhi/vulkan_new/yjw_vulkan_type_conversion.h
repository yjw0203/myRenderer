#pragma once

#include "vulkan/vulkan.h"
#include "rhi/rhi/yjw_rhi_header.h"

namespace rhi
{
    namespace VulkanConverter
    {
        VkFormat convertFormat(RHIFormat rhiFormat);

        VkShaderStageFlags convertShaderStageFlag(RHIShaderType stage);

        VkImageUsageFlags convertImageResourceUsage(RHIResourceUsage rhiUsage);

        VkBufferUsageFlags convertBufferResourceUsage(RHIResourceUsage rhiUsage);

        VkMemoryPropertyFlags convertMemoryProperty(RHIMemoryType rhiMemoryType);

        VkImageLayout convertResourceState(RHIResourceState rhiResourceState);
    }
}