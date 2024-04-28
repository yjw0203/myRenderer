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

    VulkanShader::VulkanShader(VulkanDevice* pDevice, const void* binary, int binarySize, const char* entryName)
        :VulkanDeviceObject(pDevice)
    {
        m_shader_module = CreateShaderModuleFromBinaryCode(pDevice->GetNativeDevice(), binary, binarySize);
        GenerateReflectionTable(binary, binarySize);
        m_entry_name = std::string(entryName);
    }

    VkShaderModule VulkanShader::GetNativeShaderModule()
    {
        return m_shader_module;
    }

    VulkanReflectTable& VulkanShader::GetReflectionTableByEntryName(RHIName name)
    {
        return m_entry_reflection_tables[name];
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

            std::unordered_map<RHIName, VulkanResourceBindingVariable>& reflection_table = m_entry_reflection_tables[RHIName(entryPoint.name.c_str())].resource_bindings;
            RHIShaderType shaderType = ConvertSPVExecutionModelToVulkanShaderType(entryPoint.execution_model);

            for (auto& resource : resources.uniform_buffers)
            {
                RHIName name(reflection.get_name(resource.id).c_str());
                spirv_cross::SPIRType type = reflection.get_type(resource.base_type_id);
                int setId = reflection.get_decoration(resource.id, spv::DecorationDescriptorSet);
                int binding = reflection.get_decoration(resource.id, spv::DecorationBinding);
                VulkanResourceBindingVariable& reflection_variable = reflection_table[name];
                reflection_variable.shaderType = shaderType;
                reflection_variable.setId = setId;
                reflection_variable.binding = binding;
                reflection_variable.resourceType = VulkanShaderResourceType::uniform_buffer;
            }

            for (auto& resource : resources.separate_images)
            {
                RHIName name(reflection.get_name(resource.id).c_str());
                spirv_cross::SPIRType type = reflection.get_type(resource.base_type_id);
                int setId = reflection.get_decoration(resource.id, spv::DecorationDescriptorSet);
                int binding = reflection.get_decoration(resource.id, spv::DecorationBinding);
                VulkanResourceBindingVariable& reflection_variable = reflection_table[name];
                reflection_variable.shaderType = shaderType;
                reflection_variable.setId = setId;
                reflection_variable.binding = binding;
                reflection_variable.resourceType = VulkanShaderResourceType::separate_images;
            }

            if (shaderType == RHIShaderType::vertex)
            {
                std::unordered_map<RHIName, VulkanInputVertexBindingVariable>& vertex_reflection_table = m_entry_reflection_tables[RHIName(entryPoint.name.c_str())].input_vertexes;
                int binding_index = 0;
                for (auto& resource : resources.stage_inputs)
                {
                    RHIName name(reflection.get_name(resource.id).c_str());
                    name.erase(0, 7);
                    int location = reflection.get_decoration(resource.id, spv::DecorationLocation);
                    spirv_cross::SPIRType type = reflection.get_type(resource.base_type_id);
                    VkFormat vertexFormat = VK_FORMAT_UNDEFINED;
                    int stride = 0;
                    if (type.basetype == spirv_cross::SPIRType::BaseType::Float)
                    {
                        switch (type.vecsize)
                        {
                        case 1:vertexFormat = VK_FORMAT_R32_SFLOAT; break;
                        case 2:vertexFormat = VK_FORMAT_R32G32_SFLOAT; break;
                        case 3:vertexFormat = VK_FORMAT_R32G32B32_SFLOAT; break;
                        case 4:vertexFormat = VK_FORMAT_R32G32B32A32_SFLOAT; break;
                        }
                        stride = type.vecsize * 4;
                    }
                    assert(vertexFormat != VK_FORMAT_UNDEFINED);
                    VulkanInputVertexBindingVariable& reflection_variable = vertex_reflection_table[name];
                    reflection_variable.location = location;
                    reflection_variable.format = vertexFormat;
                    reflection_variable.binding = binding_index;
                    reflection_variable.stride = stride;
                    binding_index++;
                }
            }
        }
    }

    VulkanShader::~VulkanShader()
    {
        vkDestroyShaderModule(GetDevice()->GetNativeDevice(), m_shader_module, nullptr);
    }
}