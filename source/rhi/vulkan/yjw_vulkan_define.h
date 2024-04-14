#pragma once
#include "RHI/rhi/yjw_rhi_define.h"
#include "vulkan/vulkan.h"
#define VULKAN_MAX_VERTEX_BINDING 16
#define VULKAN_MAX_DESCRIPTOR_SET 16
namespace rhi
{
    enum VulkanShaderResourceType
    {
        uniform_buffer,
        sampled_image,
        separate_images
    };

    struct VulkanInputVertexBindingVariable
    {
        int location;
        int binding;
        int stride;
        VkFormat format;
    };

    struct VulkanResourceBindingVariable
    {
        RHIShaderType shaderType;
        int setId;
        int binding;
        VulkanShaderResourceType resourceType;
    };
}