#pragma once
#include "RHI/rhi/yjw_rhi_shader.h"
#include "RHI/rhi/yjw_rhi_common.h"
#include "yjw_vulkan_device.h"
#include "yjw_vulkan_define.h"
#include <unordered_map>
#include "vulkan/vulkan.h"

namespace rhi
{
    class VulkanShader : public RHIShader, VulkanDeviceObject
    {
    public:
        VulkanShader(VulkanDevice* pDevice, const char* url, bool isBinary);
        ~VulkanShader();
        VkShaderModule GetNativeShaderModule();
        std::unordered_map<RHIName, VulkanResourceBindingVariable>& GetReflectionTableByEntryName(RHIName name);
    private:
        void ReadCodeFromFileUrl(const char* url, std::vector<char>& code);
        VkShaderModule CreateShaderModuleFromBinaryCode(VkDevice device, const void* pCode,int code_size);
        VkShaderModule CreateShaderModuleFromBinaryUrl(VkDevice device, const char* url);
        VkShaderModule CreateShaderModuleFromSourceUrl(VkDevice device, const char* url);
        void GenerateReflectionTable(const void* pBinaryCode,int code_size);
        
    private:
        VulkanDevice* m_parent_device = nullptr;
        VkShaderModule m_shader_module = nullptr;
        std::unordered_map<RHIName, std::unordered_map<RHIName, VulkanResourceBindingVariable>> m_entry_reflection_tables;
    };
}