#pragma once
#define VULKAN_MAX_DESCRIPTOR_SET 16
namespace rhi
{
    enum VulkanShaderType
    {
        vertex,
        fragment,
        compute,
        count
    };

    enum VulkanShaderResourceType
    {
        uniform_buffer,
        sampled_image
    };

    struct VulkanResourceBinding
    {
        VulkanShaderType shaderType;
        int setId;
        int binding;
        VulkanShaderResourceType resourceType;
    };
}