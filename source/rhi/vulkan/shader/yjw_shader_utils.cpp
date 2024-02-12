#include "yjw_shader_utils.h"
#include <fstream>
#include "spirv_cross/spirv_reflect.hpp"
#include "rhi/vulkan/yjw_vulkan_global.h"
#include <vector>

namespace vulkan 
{
    void loadFromSpirvFile(const char* fileName, VulkanShader* shader);

    VulkanShader* VulkanShaderAllocateStrategy::CreateFunc(const VulkanShaderCreation& creation)
    {
        VulkanShader* shader = new VulkanShader();
        loadFromSpirvFile(creation.path.c_str(), shader);
    }

    void VulkanShaderAllocateStrategy::DestoryFunc(VulkanShader* shader)
    {
        vkDestroyShaderModule(VK_G(VkDevice), shader->shaderModule, nullptr);
        delete shader;
    }

    VulkanShaderHandle ShaderPool::createShader(VulkanShaderCreation& creation)
    {
        return DefaultAllocator.create(creation);
    }
    void ShaderPool::destoryShader(VulkanShaderHandle shader)
    {
        DefaultAllocator.destory(shader);
    }

void readFile(const char* filename, std::vector<char>& result)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();

    result.resize(fileSize);

    file.seekg(0);
    file.read(result.data(), fileSize);

    file.close();
}

void getReflectFromSpirv(const char* spirvPointer, int spirvSize, std::unordered_map<std::string, ShaderReflect>& refects);
void loadFromSpirvFile(const char* fileName, VulkanShader* shader)
{
    std::vector<char> spirv;
    readFile(fileName, spirv);
    getReflectFromSpirv(spirv.data(), spirv.size(), shader->reflects);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = spirv.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(spirv.data());
    if (vkCreateShaderModule(VK_G(VkDevice), &createInfo, nullptr, &shader->shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
}

BaseDataType parseType(spirv_cross::SPIRType type)
{
    if (type.columns == 1 && type.vecsize == 2)return Vector2;
    if (type.columns == 1 && type.vecsize == 3)return Vector3;
    if (type.columns == 1 && type.vecsize == 4)return Vector4;
    if (type.columns == 3 && type.vecsize == 3)return Matrix3;
    if (type.columns == 4 && type.vecsize == 4)return Matrix4;
    switch (type.basetype)
    {
    case spirv_cross::SPIRType::BaseType::Unknown:return None;
    case spirv_cross::SPIRType::BaseType::Boolean:return Boolean;
    case spirv_cross::SPIRType::BaseType::Int:return Int;
    case spirv_cross::SPIRType::BaseType::Short:return Half;
    case spirv_cross::SPIRType::BaseType::Float:return Float;
    case spirv_cross::SPIRType::BaseType::Double:return Double;
    }
    return None;
}

VkShaderStageFlagBits parseShaderStage(spv::ExecutionModel stage)
{
    switch (stage)
    {
    case spv::ExecutionModel::ExecutionModelVertex:return VK_SHADER_STAGE_VERTEX_BIT;
    case spv::ExecutionModel::ExecutionModelGeometry:return VK_SHADER_STAGE_GEOMETRY_BIT;
    case spv::ExecutionModel::ExecutionModelFragment:return VK_SHADER_STAGE_FRAGMENT_BIT;
    case spv::ExecutionModel::ExecutionModelGLCompute:return VK_SHADER_STAGE_COMPUTE_BIT;
    case spv::ExecutionModel::ExecutionModelTaskNV:return VK_SHADER_STAGE_TASK_BIT_NV;
    case spv::ExecutionModel::ExecutionModelMeshNV:return VK_SHADER_STAGE_MESH_BIT_NV;
    default:return (VkShaderStageFlagBits)0;
    }
}


void getReflectFromSpirv(const char* spirvPointer, int spirvSize, std::unordered_map<std::string, ShaderReflect>& refects)
{
    spirv_cross::CompilerReflection reflection((uint32_t*)spirvPointer, spirvSize);
    spirv_cross::ShaderResources resources = reflection.get_shader_resources();

    spirv_cross::SmallVector<spirv_cross::EntryPoint> entry_points = reflection.get_entry_points_and_stages();

    for (spirv_cross::EntryPoint& entryPoint : entry_points)
    {
        reflection.set_entry_point(entryPoint.name, entryPoint.execution_model);

        ShaderReflect& refect = refects[entryPoint.name];

        refect.stage = parseShaderStage(entryPoint.execution_model);
        refect.setCount = 0;

        //parseUniformBuffers
        refect.blockBuffers.clear();
        for (auto& resource : resources.uniform_buffers)
        {
            BlockBuffer ub;
            ub.name = reflection.get_name(resource.id);;
            spirv_cross::SPIRType type = reflection.get_type(resource.base_type_id);

            uint32_t ub_size = 0;
            uint32_t member_count = type.member_types.size();
            for (int i = 0; i < member_count; i++)
            {
                BlockBufferMember ubm;
                ubm.name = reflection.get_member_name(resource.base_type_id, i);
                auto& member_type = reflection.get_type(type.member_types[i]);
                ubm.type = parseType(member_type);

                ubm.size = reflection.get_declared_struct_member_size(type, i);
                ubm.offset = reflection.type_struct_member_offset(type, i);

                ub_size += ubm.size;
                ub.members.push_back(ubm);
            }
            ub.size = ub_size;
            ub.set = reflection.get_decoration(resource.id, spv::DecorationDescriptorSet);
            ub.binding = reflection.get_decoration(resource.id, spv::DecorationBinding);
            if (refect.setCount <= ub.set)
            {
                refect.setCount = ub.set + 1;
            }
            refect.blockBuffers.push_back(ub);
        }

        //parseSampler2Ds
        refect.sampler2Ds.clear();
        for (auto& resource : resources.sampled_images)
        {
            Sampler2D s;
            s.name = reflection.get_name(resource.id);
            s.set = reflection.get_decoration(resource.id, spv::DecorationDescriptorSet);
            s.binding = reflection.get_decoration(resource.id, spv::DecorationBinding);
            if (refect.setCount <= s.set)
            {
                refect.setCount = s.set + 1;
            }
            refect.sampler2Ds.push_back(s);
        }
    }
}

}