#pragma once
#include "RHI/rhi/yjw_rhi_define.h"
#define VULKAN_MAX_DESCRIPTOR_SET 16
namespace rhi
{
    enum VulkanShaderResourceType
    {
        uniform_buffer,
        sampled_image
    };

    struct VulkanResourceBindingVariable
    {
        RHIShaderType shaderType;
        int setId;
        int binding;
        VulkanShaderResourceType resourceType;
    };
}