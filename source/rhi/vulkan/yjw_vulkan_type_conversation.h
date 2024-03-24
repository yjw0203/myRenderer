#pragma once
#include "vulkan/vulkan.h"
#include "yjw_vulkan_define.h"

namespace rhi
{
    VkShaderStageFlagBits ConvertShaderTypeToVkStage(VulkanShaderType shaderType);
    VkDescriptorType ConvertShaderResourceTypeToDescriptorType(VulkanShaderResourceType shaderType);
}