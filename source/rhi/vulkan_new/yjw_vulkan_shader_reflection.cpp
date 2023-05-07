#include "yjw_vulkan_shader_reflection.h"
#include "spirv_cross/spirv_reflect.hpp"

namespace rhi
{
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

    void VulkanShaderReflecter::reflect(std::vector<uint32_t>& code, VulkanShaderReflectionData& out_data)
    {
        spirv_cross::CompilerReflection* reflection = new spirv_cross::CompilerReflection(code);
        spirv_cross::ShaderResources resources = reflection->get_shader_resources();

        out_data.setCount = 0;

        //parseUniformBuffers
        for (auto& resource : resources.uniform_buffers)
        {
            BlockBuffer ub;
            ub.name = reflection->get_name(resource.id);;
            spirv_cross::SPIRType type = reflection->get_type(resource.base_type_id);

            uint32_t ub_size = 0;
            uint32_t member_count = type.member_types.size();
            for (int i = 0; i < member_count; i++)
            {
                BlockBufferMember ubm;
                ubm.name = reflection->get_member_name(resource.base_type_id, i);
                auto& member_type = reflection->get_type(type.member_types[i]);
                ubm.type = parseType(member_type);

                ubm.size = reflection->get_declared_struct_member_size(type, i);
                ubm.offset = reflection->type_struct_member_offset(type, i);

                ub_size += ubm.size;
                ub.members.push_back(ubm);
            }
            ub.size = ub_size;
            ub.set = reflection->get_decoration(resource.id, spv::DecorationDescriptorSet);
            ub.binding = reflection->get_decoration(resource.id, spv::DecorationBinding);
            if (out_data.setCount <= ub.set)
            {
                out_data.setCount = ub.set + 1;
            }
            out_data.blockBuffers.push_back(ub);
        }

        //parseSampler2Ds
        out_data.sampler2Ds.clear();
        for (auto& resource : resources.sampled_images)
        {
            Sampler2D s;
            s.name = reflection->get_name(resource.id);
            s.set = reflection->get_decoration(resource.id, spv::DecorationDescriptorSet);
            s.binding = reflection->get_decoration(resource.id, spv::DecorationBinding);
            if (out_data.setCount <= s.set)
            {
                out_data.setCount = s.set + 1;
            }
            out_data.sampler2Ds.push_back(s);
        }

        delete reflection;
    }
}
