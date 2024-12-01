#include "Engine/Engine/Public/Framework/Material/Material.h"
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"

namespace yjw
{
    void MaterialParameterPool::AddUBOLayout(rpi::RHIShaderType shaderType, rhi::ShaderReflect::UBO& ubo)
    {
        UBOBinding ubo_binding{};
        ubo_binding.name = ubo.m_name;
        ubo_binding.offset = m_size;
        ubo_binding.block_size = ubo.m_block_size;
        ubo_binding.shaderType = shaderType;
        m_ubo_bindings.push_back(ubo_binding);

        for (const rhi::ShaderReflect::UBOMember& member : ubo.m_members)
        {
            MaterialParameter parameter{};
            parameter.name = member.m_name;
            parameter.offset = ubo_binding.offset + member.m_offset;
            parameter.width = rhi::ShaderReflect::DataTypeToSize(member.m_type);
            m_parameters.push_back(parameter);
        }

        m_size += ubo.m_block_size;
        m_size = (m_size + 63) / 64 * 64;//align to 64
    }

    void MaterialParameterPool::FlushLayoutSet()
    {
        Clear();
        m_data = malloc(m_size);
        m_gpu_data = rpi::RPICreateUploadBuffer(m_size);
        for (const UBOBinding& ubo_binding : m_ubo_bindings)
        {
            m_gpu_views.push_back(rpi::RPICreateBufferView(m_gpu_data, ubo_binding.offset, ubo_binding.block_size));
        }
    }

    void MaterialParameterPool::Clear()
    {
        if (m_data)
        {
            free(m_data);
        }
        for (rpi::RPIBuffer& buffer : m_gpu_views)
        {
            if (!buffer.IsNull())
            {
                buffer.Release();
            }
        }
        m_gpu_views.clear();
        if (!m_gpu_data.IsNull())
        {
            m_gpu_data.Release();
        }
    }

    void MaterialParameterPool::SetData(const std::string& name, void* data, int size)
    {
        for (MaterialParameter& parameter : m_parameters)
        {
            if (parameter.name == name)
            {
                assert(parameter.width == size);
                memcpy((void*)((char*)m_data + parameter.offset), data, size);
                m_cpu_data_dirty = true;
            }
        }
    }
    void MaterialParameterPool::SetDataFloat(const std::string& name, float value)
    {
        SetData(name, &value, sizeof(float));
    }
    void MaterialParameterPool::SetDataVec2(const std::string& name, glm::vec2 value)
    {
        SetData(name, &value, sizeof(glm::vec2));
    }
    void MaterialParameterPool::SetDataVec3(const std::string& name, glm::vec3 value)
    {
        SetData(name, &value, sizeof(glm::vec3));
    }
    void MaterialParameterPool::SetDataVec4(const std::string& name, glm::vec4 value)
    {
        SetData(name, &value, sizeof(glm::vec4));
    }
    void MaterialParameterPool::SetDataMat4(const std::string& name, glm::mat4x4 value)
    {
        SetData(name, &value, sizeof(glm::mat4x4));
    }

    void MaterialParameterPool::FlushCpuDataToGpu()
    {
        if (m_cpu_data_dirty)
        {
            rpi::RPIUpdateBuffer(m_gpu_data, m_data, 0, m_size);
        }
    }

    Material::Material(const char* vs, const char* vs_entry, const char* ps, const char* ps_entry)
    {
        m_vs.shader_path = vs;
        m_vs.entry_name = vs_entry;
        m_ps.shader_path = ps;
        m_ps.entry_name = ps_entry;
    }

    Material::~Material()
    {
        m_pipeline->release();
    }

    void Material::BuildPipeline()
    {
        if (m_pipeline_builded == false)
        {
            using namespace rpi;
            RPIShader vs = RPICreateShader(RPIShaderType::vertex, m_vs.shader_path.c_str(), m_vs.entry_name.c_str());
            RPIShader ps = RPICreateShader(RPIShaderType::fragment, m_ps.shader_path.c_str(), m_ps.entry_name.c_str());
            RPIRenderPipelineDescriptor pipelineDesc = RPIGetDefaultRenderPipeline();
            pipelineDesc.vs = vs;
            pipelineDesc.ps = ps;
            pipelineDesc.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_depth_read_and_write);
            m_pipeline = RPICreateRenderPipeline(pipelineDesc);
            vs->release();
            ps->release();
            m_vs_reflect = m_pipeline->GetVSShaderReflect();
            m_ps_reflect = m_pipeline->GetPSShaderReflect();
            m_pipeline_builded = true;
        }
    }

    rpi::RPIPipeline Material::GetPipeline()
    {
        return m_pipeline;
    }

    MaterialInstance::MaterialInstance(Material* material)
    {
        using namespace rpi;
        m_material = material;
        m_material->BuildPipeline();
        m_resource_binding = RPICreateResourceBinding(m_material->m_pipeline);
        m_parameters_pool.Clear();
        for (rhi::ShaderReflect::UBO& ubo : m_material->m_vs_reflect->m_ubos)
        {
            if (!g_internal_shader_parameters.IsShaderParameterNameInternal(ubo.m_name))
            {
                m_parameters_pool.AddUBOLayout(rpi::RPIShaderType::vertex, ubo);
            }
            else
            {
                m_resource_binding.SetBuffer(rpi::RPIShaderType::vertex, RHIName(ubo.m_name), g_internal_shader_parameters.GetGpuBufferByShaderParameterName(ubo.m_name));
            }
        }
        for (rhi::ShaderReflect::UBO& ubo : m_material->m_ps_reflect->m_ubos)
        {
            if (!g_internal_shader_parameters.IsShaderParameterNameInternal(ubo.m_name))
            {
                m_parameters_pool.AddUBOLayout(rpi::RPIShaderType::fragment, ubo);
            }
            else
            {
                m_resource_binding.SetBuffer(rpi::RPIShaderType::fragment, RHIName(ubo.m_name), g_internal_shader_parameters.GetGpuBufferByShaderParameterName(ubo.m_name));
            }
        }
        m_parameters_pool.FlushLayoutSet();
        for (int index = 0; index < m_parameters_pool.m_ubo_bindings.size(); index++)
        {
            const MaterialParameterPool::UBOBinding& ubo_binding = m_parameters_pool.m_ubo_bindings[index];
            rpi::RPIBuffer gpu_view = m_parameters_pool.m_gpu_views[index];
            m_resource_binding.SetBuffer(ubo_binding.shaderType, ubo_binding.name, gpu_view);
        }
    }

    MaterialInstance::~MaterialInstance()
    {
        m_parameters_pool.Clear();
    }

    void MaterialInstance::SetDataFloat(const std::string& name, float value)
    {
        m_parameters_pool.SetDataFloat(name, value);
    }
    void MaterialInstance::SetDataVec2(const std::string& name, glm::vec2 value)
    {
        m_parameters_pool.SetDataVec2(name, value);
    }
    void MaterialInstance::SetDataVec3(const std::string& name, glm::vec3 value)
    {
        m_parameters_pool.SetDataVec3(name, value);
    }
    void MaterialInstance::SetDataVec4(const std::string& name, glm::vec4 value)
    {
        m_parameters_pool.SetDataVec4(name, value);
    }
    void MaterialInstance::SetDataMat4(const std::string& name, glm::mat4x4 value)
    {
        m_parameters_pool.SetDataMat4(name, value);
    }

    void MaterialInstance::SetTexture(const std::string& name, rpi::RPITexture texture)
    {
        m_resource_binding.SetTexture(rpi::RPIShaderType::vertex, name, texture);
        m_resource_binding.SetTexture(rpi::RPIShaderType::fragment, name, texture);
    }

    void MaterialInstance::FlushDataToGpu()
    {
        m_parameters_pool.FlushCpuDataToGpu();
    }

    rpi::RPIResourceBinding& MaterialInstance::GetResourceBinding()
    {
        return m_resource_binding;
    }

    rpi::RPIPipeline MaterialInstance::GetPipeline()
    {
        return m_material->GetPipeline();
    }

    Material g_pbr_material = Material(SHADER_FILE(MeshVertex.hlsl), "VSMain", SHADER_FILE(forward_pbr.hlsl), "PSMain");
    Material g_simple_mesh_pbr_material = Material(SHADER_FILE(MeshVertex.hlsl), "SimpleVS", SHADER_FILE(forward_pbr.hlsl), "PSMain");
}