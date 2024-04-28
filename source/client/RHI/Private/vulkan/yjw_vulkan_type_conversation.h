#pragma once
#include "Private/vulkan/yjw_vulkan_define.h"
#include "Public/RHI/rhi/yjw_rhi_define.h"
#include "vulkan/vulkan.h"

namespace rhi
{
    VkShaderStageFlagBits ConvertShaderTypeToVkStage(RHIShaderType shaderType);
    RHIShaderType ConvertVkStageToShaderType(VkShaderStageFlagBits shaderType);
    VkDescriptorType ConvertShaderResourceTypeToDescriptorType(VulkanShaderResourceType shaderType);
    VkBufferUsageFlags ConvertBufferUsageToVkBufferUsage(RHIResourceUsage usage);
    VkImageUsageFlags ConvertImageUsageToVkImageUsage(RHIResourceUsage usage);
    VkMemoryPropertyFlags ConvertMemoryTypeToVkMemoryPropertyFlags(RHIMemoryType memoryType);
    VkFormat ConvertFormatToVkFormat(RHIFormat format);
    VkCompareOp ConvertCompareOpToVkCompareOp(RHICompareOp compareOp);
    VkStencilOp ConvertStencilOpToVkStencilOp(RHIStencilOp stencilOp);
    VkStencilOpState ConvertStencilOpStateToVkStencilOpState(RHIStencilOpState stencilOpState);
}