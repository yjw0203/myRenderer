#include "Engine/Engine/Public/Framework/Primitive/Primitive.h"
#include "projectInfo.h"

namespace yjw
{
    void Primitive::BuildGpuPrimitive()
    {
        if (m_primitive_binding.IsNull())
        {
            rpi::RPIShader vs = rpi::RPICreateShader(rpi::RPIShaderType::vertex, SHADER_FILE(MeshVertex.hlsl), "SimpleVS");
            m_primitive_binding = rpi::RPICreatePrimitiveBinding(vs);
            m_primitive_binding.SetVertexBuffer(rhi::RHIName("POSITION"), GetVertexBuffer(VertexType::postion));
            m_primitive_binding.SetVertexBuffer(rhi::RHIName("NORMAL"), GetVertexBuffer(VertexType::normal));
            m_primitive_binding.SetVertexBuffer(rhi::RHIName("TEXCOORD0"), GetVertexBuffer(VertexType::uv));
            m_primitive_binding.SetIndexBuffer(m_index_buffer, 0, GetIndexBuffer().GetBuffer()->GetDesc().width / (m_is_indices_16bit ? 2 : 4), m_is_indices_16bit);
        }
    }

    rpi::RPIBuffer Primitive::GetVertexBuffer(VertexType type)
    {
        if (m_vertex_buffers[(int)type].IsNull())
        {
            m_vertex_buffers[(int)type] = rpi::RPICreateGpuVertexBuffer(m_vertexes[(int)type].size());
            rpi::RPIUpdateBuffer(m_vertex_buffers[(int)type], m_vertexes[(int)type].data(), 0, m_vertexes[(int)type].size());
        }
        return m_vertex_buffers[(int)type];
    }

    rpi::RPIBuffer Primitive::GetIndexBuffer()
    {
        if (m_index_buffer.IsNull())
        {
            m_index_buffer = rpi::RPICreateGpuVertexBuffer(m_indices.size());
            rpi::RPIUpdateBuffer(m_index_buffer, m_indices.data(), 0, m_indices.size());
        }
        return m_index_buffer;
    }

    rpi::RPIPrimitiveBinding Primitive::GetPrimitiveBinding()
    {
        if (m_primitive_binding.IsNull())
        {
            BuildGpuPrimitive();
        }
        return m_primitive_binding;
    }

    BoxPrimitive::BoxPrimitive()
    {
        std::vector<glm::vec3> posiitons;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;
        std::vector<glm::int32_t> indices;

        glm::vec2 temps[6] = { {0,0},{0,1},{1,1},{0,0},{1,1},{1,0} };
        for (int i = 0; i < 3; i++)
        {
            for (int j = i + 1; j < 3; j++)
            {
                int k = 3 - i - j;
                for (int n = -1; n <= 1; n += 2)
                {
                    for (int t = 0; t < 6; t++)
                    {
                        glm::vec2 uv = temps[t];
                        glm::vec3 normal = {};
                        normal[k] = (float)n;
                        glm::vec3 position = {};
                        position[i] = uv[0] * 2 - 1;
                        position[j] = uv[1] * 2 - 1;
                        position[k] = (float)n;
                    
                        posiitons.push_back(position);
                        normals.push_back(normal);
                        uvs.push_back(uv);
                    }
                }

            }
        }
        for (int i = 0; i < 36; i++)
        {
            indices.push_back(i);
        }

        m_vertexes[(int)VertexType::postion].resize(sizeof(glm::vec3) * posiitons.size());
        memcpy(m_vertexes[(int)VertexType::postion].data(), posiitons.data(), sizeof(glm::vec3) * posiitons.size());

        m_vertexes[(int)VertexType::normal].resize(sizeof(glm::vec3) * normals.size());
        memcpy(m_vertexes[(int)VertexType::normal].data(), normals.data(), sizeof(glm::vec3) * normals.size());

        m_vertexes[(int)VertexType::uv].resize(sizeof(glm::vec2) * uvs.size());
        memcpy(m_vertexes[(int)VertexType::uv].data(), uvs.data(), sizeof(glm::vec2) * uvs.size());

        m_indices.resize(sizeof(int32_t) * indices.size());
        memcpy(m_indices.data(), indices.data(), sizeof(int32_t) * indices.size());
    }

    SpherePrimitive::SpherePrimitive()
    {

    }

    BoxPrimitive g_box_primitive;
}