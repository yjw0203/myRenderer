#include "yjw_vulkan_type_conversation.h"

namespace rhi
{
    VkShaderStageFlagBits ConvertShaderTypeToVkStage(VulkanShaderType shaderType)
    {
        switch (shaderType)
        {
        case VulkanShaderType::vertex:return VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
        case VulkanShaderType::fragment:return VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
        case VulkanShaderType::compute:return VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
        }
        return (VkShaderStageFlagBits)0;
    }

    VkDescriptorType ConvertShaderResourceTypeToDescriptorType(VulkanShaderResourceType shaderType)
    {
        switch (shaderType)
        {
        case VulkanShaderResourceType::uniform_buffer:return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case VulkanShaderResourceType::sampled_image:return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        }
        return (VkDescriptorType)0;
    }
}