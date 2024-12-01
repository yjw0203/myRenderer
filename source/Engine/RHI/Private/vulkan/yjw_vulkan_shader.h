#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_shader.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_common.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_define.h"
#include "Engine/RHI/Public/shaderCompiler/yjw_shader_compiler.h"
#include <unordered_map>
#include "vulkan/vulkan.h"

namespace rhi
{
    class VulkanShader : public RHIShader, VulkanDeviceObject
    {
    public:
        VulkanShader(VulkanDevice* pDevice, const void* binary, int binarySize, const char* entryName, const ShaderReflect& reflect);
        ~VulkanShader();
        VkShaderModule GetNativeShaderModule();
        const ShaderReflect& GetReflect();
        const char* GetEntryName();
        virtual ShaderReflect* GetShaderReflect() override;

        // for vertex shader
        virtual RHIPrimitiveBinding* CreatePrimitiveBinding() override;
    private:
        void ReadCodeFromFileUrl(const char* url, std::vector<char>& code);
        VkShaderModule CreateShaderModuleFromBinaryCode(VkDevice device, const void* pCode,int code_size);
        VkShaderModule CreateShaderModuleFromBinaryUrl(VkDevice device, const char* url);
        VkShaderModule CreateShaderModuleFromSourceUrl(VkDevice device, const char* url);
    private:
        VulkanDevice* m_parent_device = nullptr;
        VkShaderModule m_shader_module = nullptr;
        std::string m_entry_name = "";
        ShaderReflect m_reflect{};
    };
}