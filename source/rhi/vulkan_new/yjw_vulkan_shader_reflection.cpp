#include "yjw_vulkan_shader_reflection.h"
#include "spirv_cross/spirv_reflect.hpp"

namespace rhi
{
    void VulkanShaderReflecter::reflect(std::vector<uint32_t>& code, VulkanShaderReflectionData& out_data)
    {
        spirv_cross::CompilerReflection* reflection = new spirv_cross::CompilerReflection(code);
        spirv_cross::ShaderResources resources = reflection->get_shader_resources();

        out_data.setCount = 0;

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
