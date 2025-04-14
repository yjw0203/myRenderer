#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"

namespace yjw
{
    InternalShaderParameter g_internal_shader_parameters;

    void InternalShaderParameter::Initialize()
    {
        m_total_size = InternalLinkType<1>::GetTotalParameterSize();
        m_gpu_data = rpi::RPICreateUploadBuffer(m_total_size);
        m_cpu_data = malloc(m_total_size);
        InternalLinkType<1>::Initialize(this);

        m_reflect = rpi::ShaderReflect{};
        for (InternalShaderParameterLayout& layout : m_shader_parameter_layouts)
        {
            rhi::ShaderReflect::UBO ubo{};
            ubo.m_binding = layout.binding_id;
            ubo.m_block_size = layout.width;
            ubo.m_name = layout.name;
            ubo.m_set = rpi::RPIGetResourceSetIDByType(rpi::RPIResourceSetType::common);
            m_reflect.m_ubos.push_back(ubo);
        }
        m_common_resource_set = rpi::RPICreateResourceSet(rpi::RPIResourceSetType::common, &m_reflect);
        for (InternalShaderParameterLayout& layout : m_shader_parameter_layouts)
        {
            m_common_resource_set.SetBuffer(layout.name, layout.gpuData);
        }
        
        rpi::RPIGlobalSetResourceSetLayout(rpi::RPIResourceSetType::common, &m_reflect);
    }

    void InternalShaderParameter::Destroy()
    {
        for (InternalShaderParameterLayout& layout : m_shader_parameter_layouts)
        {
            layout.gpuData.Release();
        }
        m_shader_parameter_layouts.clear();
        m_gpu_data.Release();
        free(m_cpu_data);
    }

    void InternalShaderParameter::FlushCpuDataToGpu()
    {
        rpi::RPIUpdateBuffer(m_gpu_data, m_cpu_data, 0, m_total_size);
    }

    rpi::RPIBuffer InternalShaderParameter::GetGpuBufferByShaderParameterName(const std::string& shaderName)
    {
        for (InternalShaderParameterLayout& layout : m_shader_parameter_layouts)
        {
            if (layout.name == shaderName)
            {
                return layout.gpuData;
            }
        }
        return rpi::RPIBuffer::Null;
    }

    bool InternalShaderParameter::IsShaderParameterNameInternal(const std::string& shaderName)
    {
        return !GetGpuBufferByShaderParameterName(shaderName).IsNull();
    }

    rpi::RPIResourceSet InternalShaderParameter::GetCommonResourceSet()
    {
        return m_common_resource_set;
    }
}