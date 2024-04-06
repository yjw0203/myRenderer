#pragma once
#include "vulkan/vulkan.h"
#include "yjw_vulkan_define.h"
#include "RHI/rhi/yjw_rhi_define.h"

namespace rhi
{
    VkShaderStageFlagBits ConvertShaderTypeToVkStage(VulkanShaderType shaderType);
    VkDescriptorType ConvertShaderResourceTypeToDescriptorType(VulkanShaderResourceType shaderType);
    VkBufferUsageFlags ConvertBufferUsageToVkBufferUsage(RHIResourceUsage usage);
    VkImageUsageFlags ConvertImageUsageToVkImageUsage(RHIResourceUsage usage);
    VkMemoryPropertyFlags ConvertMemoryTypeToVkMemoryPropertyFlags(RHIMemoryType memoryType);
    VkFormat ConvertFormatToVkFormat(RHIFormat format);
}