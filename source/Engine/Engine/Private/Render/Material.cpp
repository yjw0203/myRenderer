#include "Engine/Engine/Public/Render/Material.h"
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
        if (m_size > 0)
        {
            m_data = malloc(m_size);
            m_gpu_data = rpi::RPICreateUploadBuffer(m_size);
            for (const UBOBinding& ubo_binding : m_ubo_bindings)
            {
                m_gpu_views.push_back(rpi::RPICreateBufferView(m_gpu_data, ubo_binding.offset, ubo_binding.block_size));
            }
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

    Material::Material(const char* ps, const char* ps_entry)
    {
        m_ps.shader_path = ps;
        m_ps.entry_name = ps_entry;
    }

    Material::~Material()
    {
        m_pixel_shader->release();
    }

    rpi::RPIShader Material::GetPixelShader()
    {
        return m_pixel_shader;
    }

    void Material::Build()
    {
        if (m_builded == false)
        {
            using namespace rpi;
            m_pixel_shader = RPICreateShader(RPIShaderType::fragment, m_ps.shader_path.c_str(), m_ps.entry_name.c_str());
            m_ps_reflect = m_pixel_shader->GetShaderReflect();
            m_builded = true;
        }
    }

    MaterialInstance::MaterialInstance(Material* material)
    {
        using namespace rpi;
        m_material = material;
        m_material->Build();
        m_parameters_pool.Clear();

        m_ps_resource_set = RPICreateResourceSet(RPIResourceSetType::ps, m_material->m_ps_reflect);

        for (rhi::ShaderReflect::UBO& ubo : m_material->m_ps_reflect->m_ubos)
        {
            if (RPICheckResourceSetTypeID(ubo.m_set, RPIResourceSetType::ps))
            {
                m_parameters_pool.AddUBOLayout(rpi::RPIShaderType::fragment, ubo);
            }
        }
        m_parameters_pool.FlushLayoutSet();
        for (int index = 0; index < m_parameters_pool.m_ubo_bindings.size(); index++)
        {
            const MaterialParameterPool::UBOBinding& ubo_binding = m_parameters_pool.m_ubo_bindings[index];
            rpi::RPIBuffer gpu_view = m_parameters_pool.m_gpu_views[index];
            m_ps_resource_set.SetBuffer(ubo_binding.name, gpu_view);
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
        m_ps_resource_set.SetTexture(name, texture);
    }

    void MaterialInstance::FlushDataToGpu()
    {
        m_parameters_pool.FlushCpuDataToGpu();
    }

    rpi::RPIResourceSet& MaterialInstance::GetResourceSet()
    {
        return m_ps_resource_set;
    }

    rpi::RPIShader MaterialInstance::GetPixelShader()
    {
        return m_material->m_pixel_shader;
    }

    Material g_pbr_material = Material(SHADER_FILE(ForwardPBR.hlsl), "PSMain");
    Material g_simple_mesh_pbr_material = Material(SHADER_FILE(ForwardPBR.hlsl), "PSMain");
}