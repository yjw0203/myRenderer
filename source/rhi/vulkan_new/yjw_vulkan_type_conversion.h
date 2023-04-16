#pragma once

#include "vulkan/vulkan.h"
#include "rhi/rhi/yjw_rhi_header.h"

namespace rhi
{
    namespace VulkanConverter
    {
        VkFormat convertFormat(RHIFormat rhiFormat);

        VkImageUsageFlags convertResourceUsage(RHIResourceUsage rhiUsage);

        VkMemoryPropertyFlags convertMemoryProperty(RHIMemoryType rhiMemoryType);

        VkImageLayout convertResourceState(RHIResourceState rhiResourceState);
    }
}