#include "Engine/Render/Private/Primitive.h"
#include "Engine/Engine/Public/Asset/Material.h"
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
#include "projectInfo.h"

namespace yjw
{
    rhi::RHIName GetMeshVertexTypeName(MeshVertexType type)
    {
        switch (type)
        {
        case MeshVertexType::UNKNOW: return "UNKNOW";
        case MeshVertexType::POSITION:return "POSITION";
        case MeshVertexType::NORMAL:return "NORMAL";
        case MeshVertexType::TARGENT:return "TARGENT";
        case MeshVertexType::UV0:return "UV0";
        case MeshVertexType::UV1:return "UV1";
        case MeshVertexType::BLEND_INDICES:return "BLEND_INDICES";
        case MeshVertexType::BLEND_WEIGHTS:return "BLEND_WEIGHTS";
        case MeshVertexType::BLEND_TYPE:return "BLEND_TYPE";
        }
        return "UNKNOW";
    }

    void Primitive::Build(const char* url)
    {
        m_mesh_ast.SetURL(url);
        BuildGpuPrimitive();
    }

    void Primitive::BuildGpuPrimitive()
    {
        MeshAST* mesh_ptr = m_mesh_ast.GetData();
        if (m_primitive_binding.IsNull() && mesh_ptr)
        {
            m_vertex_buffers.resize(mesh_ptr->m_vertex_buffers.size());
            for (int i = 0; i < m_vertex_buffers.size(); i++)
            {
                m_vertex_buffers[i].m_type = mesh_ptr->m_vertex_buffers[i].m_type;
                m_vertex_buffers[i].m_buffer = rpi::RPICreateGpuVertexBuffer(mesh_ptr->m_vertex_buffers[i].m_vertexes.size());
                rpi::RPIUpdateBuffer(m_vertex_buffers[i].m_buffer, mesh_ptr->m_vertex_buffers[i].m_vertexes.data(), 0, mesh_ptr->m_vertex_buffers[i].m_vertexes.size());
            }

            m_index_buffer.m_is_indices_16bit = mesh_ptr->m_is_index_short;
            m_index_buffer.m_buffer = rpi::RPICreateGpuVertexBuffer(mesh_ptr->m_index_buffer.size());
            rpi::RPIUpdateBuffer(m_index_buffer.m_buffer, mesh_ptr->m_index_buffer.data(), 0, mesh_ptr->m_index_buffer.size());

            m_vertex_shader = rpi::RPICreateShader(rpi::RPIShaderType::vertex, SHADER_FILE(MeshVertex.hlsl), "SimpleVS");
            m_vs_resource_set = rpi::RPICreateResourceSet(rpi::RPIResourceSetType::vs, m_vertex_shader->GetShaderReflect());
            m_primitive_binding = rpi::RPICreatePrimitiveBinding(m_vertex_shader);
            for (int i = 0; i < m_vertex_buffers.size(); i++)
            {
                m_primitive_binding.SetVertexBuffer(GetMeshVertexTypeName(m_vertex_buffers[i].m_type), m_vertex_buffers[i].m_buffer);
            }
        
            std::map<std::string, int> material_slot_index;
            for (std::pair<const std::string, AssetReferece<MaterialInstanceAST>>& itr : mesh_ptr->m_materials)
            {
                MaterialInstanceAST* mi_ast = itr.second.GetData();
                if (mi_ast)
                {
                   MaterialAST* m_ast = mi_ast->m_material_template.GetData();
                   if (m_ast)
                   {
                       material_slot_index[itr.first] = m_materials.size();
                       Material* material = new Material();
                       material->Build((std::string{} + SHADER_PATH + "/" + m_ast->m_shader).c_str(), m_ast->m_entry.c_str());
                       m_materials.push_back(material);
                       MaterialInstance* material_instance = new MaterialInstance();
                       material_instance->Build(m_materials.back());
                       m_material_instances.push_back(material_instance);
                       for (std::pair<const std::string, float>& param : mi_ast->m_float_params)
                       {
                           m_material_instances.back()->SetDataFloat(param.first, param.second);
                       }
                       for (std::pair<const std::string, std::string>& param : mi_ast->m_texture_params)
                       {
                           rpi::RPITexture texture = rpi::RPICreateTexture2DFromFile((std::string{} + RESOURCE_PATH + "/" + param.second).c_str());
                           m_material_instances.back()->SetTexture(param.first, texture);
                       }
                       m_material_instances.back()->FlushDataToGpu();
                    }
                }
            }

            for (SubMeshInfo& sub_mesh : mesh_ptr->m_sub_meshes)
            {
                SubPrimitive sub_primitive{};
                sub_primitive.m_sub_primitive_id = m_primitive_binding.AddIndexBuffer(m_index_buffer.m_buffer, sub_mesh.m_start_index, sub_mesh.m_index_count, m_index_buffer.m_is_indices_16bit);
                if (material_slot_index.find(sub_mesh.m_material_slot) != material_slot_index.end())
                {
                    sub_primitive.m_material = m_material_instances[material_slot_index[sub_mesh.m_material_slot]];
                }
                else
                {
                    sub_primitive.m_material = g_default_material_instance;
                }

                sub_primitive.m_material_slot = sub_mesh.m_material_slot;
                m_sub_primitives.push_back(sub_primitive);
            }
        }
    }

    Primitive::~Primitive()
    {
        m_vertex_shader->release();
        m_vs_resource_set.Release();
        m_primitive_binding.Release();
        for (int i = 0; i < m_vertex_buffers.size(); i++)
        {
            if (m_vertex_buffers[i].m_buffer)
            {
                m_vertex_buffers[i].m_buffer.Release();
            }
        }
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

    const std::vector<SubPrimitive>& Primitive::GetSubPrimitive()
    {
        return m_sub_primitives;
    }

    BoxPrimitive::BoxPrimitive()
    {
        m_mesh_ast.SetURL("Content/box.mesh.ast");

        
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
        MeshAST* mesh_ptr = m_mesh_ast.GetData();
        if (mesh_ptr)
        {
            mesh_ptr->m_vertex_buffers.resize(3);
            mesh_ptr->m_vertex_buffers[0].m_type = MeshVertexType::POSITION;
            mesh_ptr->m_vertex_buffers[0].m_vertexes.resize(sizeof(glm::vec3) * posiitons.size());
            memcpy(mesh_ptr->m_vertex_buffers[0].m_vertexes.data(), posiitons.data(), sizeof(glm::vec3) * posiitons.size());

            mesh_ptr->m_vertex_buffers[1].m_type = MeshVertexType::NORMAL;
            mesh_ptr->m_vertex_buffers[1].m_vertexes.resize(sizeof(glm::vec3) * normals.size());
            memcpy(mesh_ptr->m_vertex_buffers[1].m_vertexes.data(), normals.data(), sizeof(glm::vec3) * normals.size());

            mesh_ptr->m_vertex_buffers[2].m_type = MeshVertexType::UV0;
            mesh_ptr->m_vertex_buffers[2].m_vertexes.resize(sizeof(glm::vec2) * uvs.size());
            memcpy(mesh_ptr->m_vertex_buffers[2].m_vertexes.data(), uvs.data(), sizeof(glm::vec2) * uvs.size());

            mesh_ptr->m_is_index_short = false;
            mesh_ptr->m_index_buffer.resize(sizeof(int32_t) * indices.size());
            memcpy(mesh_ptr->m_index_buffer.data(), indices.data(), sizeof(int32_t) * indices.size());

            mesh_ptr->m_sub_meshes.push_back(SubMeshInfo{ 0,(int)indices.size(),"default" });

            m_mesh_ast.Save();
        }
        
    }

    SpherePrimitive::SpherePrimitive()
    {

    }

    BoxPrimitive g_box_primitive;
}