#pragma once
#include "Public/RHI/rhi/yjw_rhi_shader.h"
#include "Public/RHI/rhi/yjw_rhi_common.h"
#include "Private/vulkan/yjw_vulkan_device.h"
#include "Private/vulkan/yjw_vulkan_define.h"
#include <unordered_map>
#include "vulkan/vulkan.h"

namespace rhi
{
    struct VulkanReflectTable
    {
        std::unordered_map<RHIName, VulkanInputVertexBindingVariable> input_vertexes;
        std::unordered_map<RHIName, VulkanResourceBindingVariable> resource_bindings;
    };

    class VulkanShader : public RHIShader, VulkanDeviceObject
    {
    public:
        VulkanShader(VulkanDevice* pDevice, const void* binary, int binarySize, const char* entryName);
        ~VulkanShader();
        VkShaderModule GetNativeShaderModule();
        VulkanReflectTable& GetReflectionTableByEntryName(RHIName name);
        const char* GetEntryName();
    private:
        void ReadCodeFromFileUrl(const char* url, std::vector<char>& code);
        VkShaderModule CreateShaderModuleFromBinaryCode(VkDevice device, const void* pCode,int code_size);
        VkShaderModule CreateShaderModuleFromBinaryUrl(VkDevice device, const char* url);
        VkShaderModule CreateShaderModuleFromSourceUrl(VkDevice device, const char* url);
        void GenerateReflectionTable(const void* pBinaryCode,int code_size);
    private:
        VulkanDevice* m_parent_device = nullptr;
        VkShaderModule m_shader_module = nullptr;
        std::string m_entry_name = "";
        std::unordered_map<RHIName, VulkanReflectTable> m_entry_reflection_tables;
    };
}