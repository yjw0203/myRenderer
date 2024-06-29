#pragma once
#include "Private/vulkan/yjw_vulkan_define.h"
#include "Public/RHI/rhi/yjw_rhi_define.h"
#include "vulkan/vulkan.h"
#include "RHI/shaderCompiler/yjw_shader_compiler.h"

namespace rhi
{
    VkShaderStageFlagBits ConvertShaderTypeToVkStage(RHIShaderType shaderType);
    RHIShaderType ConvertVkStageToShaderType(VkShaderStageFlagBits shaderType);
    VkBufferUsageFlags ConvertBufferUsageToVkBufferUsage(RHIResourceUsage usage);
    VkImageUsageFlags ConvertImageUsageToVkImageUsage(RHIResourceUsage usage);
    VkMemoryPropertyFlags ConvertMemoryTypeToVkMemoryPropertyFlags(RHIMemoryType memoryType);
    VkFormat ConvertFormatToVkFormat(RHIFormat format);
    RHIFormat ConvertVkFormatToRHIFormat(VkFormat format);
    VkCompareOp ConvertCompareOpToVkCompareOp(RHICompareOp compareOp);
    VkStencilOp ConvertStencilOpToVkStencilOp(RHIStencilOp stencilOp);
    VkStencilOpState ConvertStencilOpStateToVkStencilOpState(RHIStencilOpState stencilOpState);
    VkFormat ConvertDataTypeToVkFormat(ShaderReflect::DataType type);
}