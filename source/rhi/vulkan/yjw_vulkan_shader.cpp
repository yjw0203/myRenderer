#include "yjw_vulkan_shader.h"
#include "spirv_cross/spirv_reflect.hpp"
#include <fstream>
#include <iostream>

namespace rhi
{
    VulkanShaderType ConvertSPVExecutionModelToVulkanShaderType(spv::ExecutionModel model)
    {
        switch (model)
        {
        case spv::ExecutionModel::ExecutionModelVertex:return VulkanShaderType::vertex;
        case spv::ExecutionModel::ExecutionModelFragment:return VulkanShaderType::fragment;
        case spv::ExecutionModel::ExecutionModelKernel:return VulkanShaderType::compute;
        }
        return VulkanShaderType::count;
    }

    VulkanShader::VulkanShader(VulkanDevice* pDevice, const char* url, bool isBinary)
        :VulkanDeviceObject(pDevice)
    {
        VkDevice device = pDevice->GetNativeDevice();
        if (isBinary)
        {
            m_shader_module = CreateShaderModuleFromBinaryUrl(device, url);
        }
        else
        {

        }
    }

    VkShaderModule VulkanShader::GetNativeShaderModule()
    {
        return m_shader_module;
    }

    std::unordered_map<RHIName, VulkanResourceBinding>& VulkanShader::GetReflectionTableByEntryName(RHIName name)
    {
        return m_entry_reflection_tables[name];
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
        GenerateReflectionTable(code.data(),code.size());
        return CreateShaderModuleFromBinaryCode(device, code.data(), code.size());
    }

    VkShaderModule VulkanShader::CreateShaderModuleFromSourceUrl(VkDevice device, const char* url)
    {
        return nullptr;
    }

    void VulkanShader::GenerateReflectionTable(const void* pBinaryCode, int code_size)
    {
        spirv_cross::CompilerReflection reflection((uint32_t*)pBinaryCode, code_size/sizeof(uint32_t));
        spirv_cross::ShaderResources resources = reflection.get_shader_resources();

        spirv_cross::SmallVector<spirv_cross::EntryPoint> entry_points = reflection.get_entry_points_and_stages();

        for (spirv_cross::EntryPoint& entryPoint : entry_points)
        {
            reflection.set_entry_point(entryPoint.name, entryPoint.execution_model);

            std::unordered_map<RHIName, VulkanResourceBinding>& reflection_table = m_entry_reflection_tables[RHIName(entryPoint.name.c_str())];
            VulkanShaderType shaderType = ConvertSPVExecutionModelToVulkanShaderType(entryPoint.execution_model);

            for (auto& resource : resources.uniform_buffers)
            {
                RHIName name(reflection.get_name(resource.id).c_str());
                spirv_cross::SPIRType type = reflection.get_type(resource.base_type_id);
                int setId = reflection.get_decoration(resource.id, spv::DecorationDescriptorSet);
                int binding = reflection.get_decoration(resource.id, spv::DecorationBinding);
                VulkanResourceBinding& reflection_variable = reflection_table[name];
                reflection_variable.shaderType = shaderType;
                reflection_variable.setId = setId;
                reflection_variable.binding = binding;
                reflection_variable.resourceType = VulkanShaderResourceType::uniform_buffer;
            }

            for (auto& resource : resources.sampled_images)
            {
                RHIName name(reflection.get_name(resource.id).c_str());
                spirv_cross::SPIRType type = reflection.get_type(resource.base_type_id);
                int setId = reflection.get_decoration(resource.id, spv::DecorationDescriptorSet);
                int binding = reflection.get_decoration(resource.id, spv::DecorationBinding);
                VulkanResourceBinding& reflection_variable = reflection_table[name];
                reflection_variable.shaderType = shaderType;
                reflection_variable.setId = setId;
                reflection_variable.binding = binding;
                reflection_variable.resourceType = VulkanShaderResourceType::sampled_image;
            }
        }
    }

    VulkanShader::~VulkanShader()
    {
        vkDestroyShaderModule(GetDevice()->GetNativeDevice(), m_shader_module, nullptr);
    }
}