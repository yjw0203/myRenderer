#pragma once

#include "rhi/rhi/yjw_rhi_shader.h"
#include "yjw_vulkan_shader_reflection.h"
#include "vulkan/vulkan.h"

namespace rhi
{
    struct VulkanShaderDesc
    {
    };

    class VulkanShaderLocation : public RHIShaderLocation
    {
        friend class VulkanShaderManager;
    public:
        const VulkanShaderDesc& getDesc();
        VkShaderModule getShaderModule();
    private:
        VulkanShaderDesc desc;
        VkShaderModule shaderModule;
    };

    class VulkanShaderManager
    {
    public:
        static VulkanShaderManager& Get()
        {
            static VulkanShaderManager instance;
            return instance;
        }

        RHIShaderLocation* createShader(const RHIShaderDesc& rhi_desc);
        void deleteShader(RHIShaderLocation*& location);
    };
}