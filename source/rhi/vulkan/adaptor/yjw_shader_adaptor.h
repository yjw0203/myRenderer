#pragma once
#include "rhi/new_rhi/yjw_rhi_shader.h"
#include "rhi/vulkan/shader/yjw_shader_utils.h"
namespace vulkan
{
    class VulkanShaderCreationAdaptor
    {
    public:
        VulkanShaderCreationAdaptor(const rhi::RHIShaderCreation& creation);
        operator VulkanShaderCreation& () { return frac; }
    private:
        VulkanShaderCreation frac;
    };
}