#include "Engine/Engine/Public/Render/Primitive.h"
#include "projectInfo.h"

namespace yjw
{
    void Primitive::BuildGpuPrimitive()
    {
        if (m_primitive_binding.IsNull() && m_mesh_ast.GetData())
        {
            m_vertex_shader = rpi::RPICreateShader(rpi::RPIShaderType::vertex, SHADER_FILE(MeshVertex.hlsl), "SimpleVS");
            m_vs_resource_set = rpi::RPICreateResourceSet(rpi::RPIResourceSetType::vs, m_vertex_shader->GetShaderReflect());
            m_primitive_binding = rpi::RPICreatePrimitiveBinding(m_vertex_shader);
            m_primitive_binding.SetVertexBuffer(rhi::RHIName("POSITION"), GetVertexBuffer(VertexType::postion));
            m_primitive_binding.SetVertexBuffer(rhi::RHIName("NORMAL"), GetVertexBuffer(VertexType::normal));
            m_primitive_binding.SetVertexBuffer(rhi::RHIName("TEXCOORD0"), GetVertexBuffer(VertexType::uv));
            m_primitive_binding.SetIndexBuffer(m_index_buffer, 0, GetIndexBuffer().GetBuffer()->GetDesc().width / (m_is_indices_16bit ? 2 : 4), m_is_indices_16bit);
        }
    }

    Primitive::~Primitive()
    {
        m_vertex_shader->release();
        m_vs_resource_set.Release();
        m_primitive_binding.Release();
        for (int i = 0; i < (int)VertexType::count; i++)
        {
            if (m_vertex_buffers[i])
            {
                m_vertex_buffers[i].Release();
            }
        }
    }

    rpi::RPIBuffer Primitive::GetVertexBuffer(VertexType type)
    {
        if (m_vertex_buffers[(int)type].IsNull())
        {
            m_vertex_buffers[(int)type] = rpi::RPICreateGpuVertexBuffer(m_mesh_ast.GetData()->m_vertexes[(int)type].size());
            rpi::RPIUpdateBuffer(m_vertex_buffers[(int)type], m_mesh_ast.GetData()->m_vertexes[(int)type].data(), 0, m_mesh_ast.GetData()->m_vertexes[(int)type].size());
        }
        return m_vertex_buffers[(int)type];
    }

    rpi::RPIBuffer Primitive::GetIndexBuffer()
    {
        if (m_index_buffer.IsNull())
        {
            m_index_buffer = rpi::RPICreateGpuVertexBuffer(m_mesh_ast.GetData()->m_indices.size());
            rpi::RPIUpdateBuffer(m_index_buffer, m_mesh_ast.GetData()->m_indices.data(), 0, m_mesh_ast.GetData()->m_indices.size());
        }
        return m_index_buffer;
    }

    rpi::RPIPrimitiveBinding Primitive::GetPrimitiveBinding()
    {
        if (m_primitive_binding.IsNull())
        {
            if (m_mesh_ast.GetData())
            {
                BuildGpuPrimitive();
            }
        }
        return m_primitive_binding;
    }

    rpi::RPIShader Primitive::GetVertexShader()
    {
        return m_vertex_shader;
    }

    rpi::RPIResourceSet Primitive::GetVSResourceSet()
    {
        return m_vs_resource_set;
    }

    BoxPrimitive::BoxPrimitive()
    {
        m_mesh_ast.SetURL("Content/box.mesh.ast");

        /*
        std::vector<glm::vec3> posiitons = {
            glm::vec3(0, 0, 0),
            glm::vec3(0, 0, 1),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 1, 1),

            glm::vec3(1, 0, 0),
            glm::vec3(1, 0, 1),
            glm::vec3(1, 1, 0),
            glm::vec3(1, 1, 1),

            glm::vec3(0, 0, 0),
            glm::vec3(0, 0, 1),
            glm::vec3(1, 0, 0),
            glm::vec3(1, 0, 1),

            glm::vec3(0, 1, 0),
            glm::vec3(0, 1, 1),
            glm::vec3(1, 1, 0),
            glm::vec3(1, 1, 1),

            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(1, 0, 0),
            glm::vec3(1, 1, 0),

            glm::vec3(0, 0, 1),
            glm::vec3(0, 1, 1),
            glm::vec3(1, 0, 1),
            glm::vec3(1, 1, 1),
        };
        std::vector<glm::vec2> uvs = {
            glm::vec2(0, 0),
            glm::vec2(1, 0),
            glm::vec2(1, 1),
            glm::vec2(0, 1),

            glm::vec2(0, 1),
            glm::vec2(1, 1),
            glm::vec2(1, 0),
            glm::vec2(0, 0),

            glm::vec2(0, 0),
            glm::vec2(1, 0),
            glm::vec2(0, 1),
            glm::vec2(1, 1),

            glm::vec2(1, 1),
            glm::vec2(0, 1),
            glm::vec2(1, 0),
            glm::vec2(0, 0),

            glm::vec2(0, 0),
            glm::vec2(1, 1),
            glm::vec2(0, 1),
            glm::vec2(1, 0),

            glm::vec2(1, 0),
            glm::vec2(0, 1),
            glm::vec2(1, 1),
            glm::vec2(0, 0),
        };
        std::vector<glm::vec3> normals = {
            glm::vec3(-1,0,0),glm::vec3(-1,0,0),glm::vec3(-1,0,0),glm::vec3(-1,0,0),
            glm::vec3(1,0,0),glm::vec3(1,0,0),glm::vec3(1,0,0),glm::vec3(1,0,0),
            glm::vec3(0,-1,0),glm::vec3(0,-1,0),glm::vec3(0,-1,0),glm::vec3(0,-1,0),
            glm::vec3(0,1,0),glm::vec3(0,1,0),glm::vec3(0,1,0),glm::vec3(0,1,0),
            glm::vec3(0,0,-1),glm::vec3(0,0,-1),glm::vec3(0,0,-1),glm::vec3(0,0,-1),
            glm::vec3(0,0,1),glm::vec3(0,0,1),glm::vec3(0,0,1),glm::vec3(0,0,1),
        };
        std::vector<glm::int32_t> indices = {
            0,1,3,0,3,2,
            4,6,7,4,7,5,
            8,11,9,8,10,11,
            14,12,13,14,13,15,
            16,17,19,16,19,18,
            20,22,21,22,23,21
        };

        AssetManager::Get()->process();
        if (m_mesh_ast.GetData())
        {
            m_mesh_ast.GetData()->m_vertexes.resize((int)VertexType::count);
            m_mesh_ast.GetData()->m_vertexes[(int)VertexType::postion].resize(sizeof(glm::vec3) * posiitons.size());
            memcpy(m_mesh_ast.GetData()->m_vertexes[(int)VertexType::postion].data(), posiitons.data(), sizeof(glm::vec3) * posiitons.size());

            m_mesh_ast.GetData()->m_vertexes[(int)VertexType::normal].resize(sizeof(glm::vec3) * normals.size());
            memcpy(m_mesh_ast.GetData()->m_vertexes[(int)VertexType::normal].data(), normals.data(), sizeof(glm::vec3) * normals.size());

            m_mesh_ast.GetData()->m_vertexes[(int)VertexType::uv].resize(sizeof(glm::vec2) * uvs.size());
            memcpy(m_mesh_ast.GetData()->m_vertexes[(int)VertexType::uv].data(), uvs.data(), sizeof(glm::vec2) * uvs.size());

            m_mesh_ast.GetData()->m_indices.resize(sizeof(int32_t) * indices.size());
            memcpy(m_mesh_ast.GetData()->m_indices.data(), indices.data(), sizeof(int32_t) * indices.size());

            m_mesh_ast.Save();
        }
        */
    }

    SpherePrimitive::SpherePrimitive()
    {

    }

    BoxPrimitive g_box_primitive;
}