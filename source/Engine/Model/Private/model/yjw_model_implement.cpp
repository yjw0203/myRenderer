#pragma once
#include "yjw_model_implement.h"

namespace yjw
{
    std::shared_ptr<CPUModel> ModelBuildImplement::Build(const std::string& filePath, const std::string& fileName)
    {
        m_cpu_model = std::make_shared<CPUModel>();
        FillModelData(filePath, fileName);
        return m_cpu_model;
    }

    int ModelBuildImplement::AddBufferView(int buffer_id, int offset, int width)
    {
        int buffer_view_id = m_cpu_model->m_pool_buffer_views.size();
        m_cpu_model->m_pool_buffer_views.push_back(CPUModel::CPUBufferView{});
        m_cpu_model->m_pool_buffer_views.back().buffer_id = buffer_id;
        m_cpu_model->m_pool_buffer_views.back().offset = offset;
        m_cpu_model->m_pool_buffer_views.back().width = width;
        return buffer_view_id;
    }

    int ModelBuildImplement::AddBuffer(MeshVertexType type, void* data, int width)
    {
        int buffer_id = m_cpu_model->m_pool_buffers.size();
        m_cpu_model->m_pool_buffers.push_back(CPUModel::CPUBuffer{});
        m_cpu_model->m_pool_buffers.back().type = type;
        m_cpu_model->m_pool_buffers.back().buffer.resize(width);
        memcpy(m_cpu_model->m_pool_buffers.back().buffer.data(), data, width);
        return buffer_id;
    }

    int ModelBuildImplement::AddBufferView(int buffer_id)
    {
        int buffer_view_id = m_cpu_model->m_pool_buffer_views.size();
        m_cpu_model->m_pool_buffer_views.push_back(CPUModel::CPUBufferView{});
        m_cpu_model->m_pool_buffer_views.back().buffer_id = buffer_id;
        m_cpu_model->m_pool_buffer_views.back().offset = 0;
        m_cpu_model->m_pool_buffer_views.back().width = m_cpu_model->m_pool_buffers[buffer_id].buffer.size();
        return buffer_view_id;
    }

    int ModelBuildImplement::GetBufferIDFromBufferView(int buffer_view_id)
    {
        return m_cpu_model->m_pool_buffer_views[buffer_view_id].buffer_id;
    }

    void ModelBuildImplement::SetBufferType(int buffer_id, MeshVertexType type)
    {
        m_cpu_model->m_pool_buffers[buffer_id].type = type;
    }

    int ModelBuildImplement::AddTexture(const std::string& texture_name)
    {
        for (int id = 0; id < m_cpu_model->m_pool_textures.size(); id++)
        {
            if (texture_name == m_cpu_model->m_pool_textures[id].texture_name)
            {
                return id;
            }
        }
        int id = m_cpu_model->m_pool_textures.size();
        m_cpu_model->m_pool_textures.push_back(CPUModel::Texture{});
        m_cpu_model->m_pool_textures.back().texture_name = texture_name;
        return id;
    }

    int ModelBuildImplement::AddTexture(const std::string& texture_name, int width, int height, void* data, int size)
    {
        int id = m_cpu_model->m_pool_textures.size();
        m_cpu_model->m_pool_textures.push_back(CPUModel::Texture{});
        m_cpu_model->m_pool_textures.back().texture_name = texture_name;
        return id;
    }

    int ModelBuildImplement::AddMesh()
    {
        int mesh_id = m_cpu_model->m_meshes.size();
        m_cpu_model->m_meshes.push_back(CPUModel::CPUMesh{});
        return mesh_id;
    }

    void ModelBuildImplement::AddIndexBuffer(int mesh_id, int buffer_view_id, int first_index, int index_count, bool is_indices_16bit /*= false*/)
    {
        m_cpu_model->m_meshes[mesh_id].first_index = first_index;
        m_cpu_model->m_meshes[mesh_id].index_count = index_count;
        m_cpu_model->m_meshes[mesh_id].index_buffer = buffer_view_id;
        m_cpu_model->m_meshes[mesh_id].is_indices_16bit = is_indices_16bit;
    }

    void ModelBuildImplement::AddVertexBuffer(int mesh_id, MeshVertexType type, int buffer_view_id)
    {
        CPUModel::VertexBuffer vertex_buffer{};
        vertex_buffer.buffer = buffer_view_id;
        vertex_buffer.type = type;
        m_cpu_model->m_meshes[mesh_id].vertex_buffers.push_back(vertex_buffer);
    }

    int ModelBuildImplement::AddMaterial(const CPUModel::Material& material)
    {
        int material_id = m_cpu_model->m_material.size();
        m_cpu_model->m_material.push_back(material);
        return material_id;
    }

    int ModelBuildImplement::AddMaterialInstance(const CPUModel::MaterialIns& material_instance)
    {
        int material_id = m_cpu_model->m_material_instances.size();
        m_cpu_model->m_material_instances.push_back(material_instance);
        return material_id;
    }

    int ModelBuildImplement::AddEntity(int mesh_id, int material_id)
    {
        int entity_id = m_cpu_model->m_entities.size();
        m_cpu_model->m_entities.push_back(CPUModel::Entity{});
        m_cpu_model->m_entities.back().mesh_id = mesh_id;
        m_cpu_model->m_entities.back().material_ins_id = material_id;
        return entity_id;
    }

    void ModelBuildImplement::AddBone(const RawBone& bone)
    {
        m_cpu_model->m_skeleton_data.m_bones_map.push_back(bone);
    }

    const RawBone& ModelBuildImplement::GetBone(int index)
    {
        return m_cpu_model->m_skeleton_data.m_bones_map[index];
    }

    SkeletonAST& ModelBuildImplement::GetSkeleton()
    {
        return m_cpu_model->m_skeleton_data;
    }

    AnimationClipAST& ModelBuildImplement::GetAnimationClip()
    {
        return m_cpu_model->m_animation_clip_data;
    }
}