#include "Private/vulkan/yjw_vulkan_shader.h"
#include "spirv_cross/spirv_reflect.hpp"
#include <fstream>
#include <iostream>

namespace rhi
{
    RHIShaderType ConvertSPVExecutionModelToVulkanShaderType(spv::ExecutionModel model)
    {
        switch (model)
        {
        case spv::ExecutionModel::ExecutionModelVertex:return RHIShaderType::vertex;
        case spv::ExecutionModel::ExecutionModelFragment:return RHIShaderType::fragment;
        case spv::ExecutionModel::ExecutionModelKernel:return RHIShaderType::compute;
        }
        return RHIShaderType::count;
    }

    VulkanShader::VulkanShader(VulkanDevice* pDevice, const void* binary, int binarySize, const char* entryName, const ShaderReflect& reflect)
        :VulkanDeviceObject(pDevice)
    {
        m_shader_module = CreateShaderModuleFromBinaryCode(pDevice->GetNativeDevice(), binary, binarySize);
        m_entry_name = std::string(entryName);
        m_reflect = reflect;
    }

    VkShaderModule VulkanShader::GetNativeShaderModule()
    {
        return m_shader_module;
    }

    const ShaderReflect& VulkanShader::GetReflect()
    {
        return m_reflect;
    }

    const char* VulkanShader::GetEntryName()
    {
        return m_entry_name.c_str();
    }

    void VulkanShader::ReadCodeFromFileUrl(const char* url, std::vector<char>& code)
    {
        std::string filename(url);
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cout << "无法打开文件：" << filename << std::endl;
            return ;
        }
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        code.resize(fileSize);
        file.read(code.data(), fileSize);
        file.close();
    }

    VkShaderModule VulkanShader::CreateShaderModuleFromBinaryCode(VkDevice device, const void* pCode, int code_size)
    {
        VkShaderModule shaderModule{};
        VkShaderModuleCreateInfo createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pCode = (const uint32_t*)pCode;
        createInfo.codeSize = code_size;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
        return shaderModule;
    }

    VkShaderModule VulkanShader::CreateShaderModuleFromBinaryUrl(VkDevice device, const char* url)
    {
        std::vector<char> code;
        ReadCodeFromFileUrl(url, code);
        return CreateShaderModuleFromBinaryCode(device, code.data(), code.size());
    }

    VkShaderModule VulkanShader::CreateShaderModuleFromSourceUrl(VkDevice device, const char* url)
    {
        return nullptr;
    }

    VulkanShader::~VulkanShader()
    {
        vkDestroyShaderModule(GetDevice()->GetNativeDevice(), m_shader_module, nullptr);
    }
}