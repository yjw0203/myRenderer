#include "Public/Model/yjw_model.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "Utils/yjw_unicode_util.h"
#include "projectInfo.h"
#include "rhi/rpi/yjw_rpi_header.h"
#include <glm/glm.hpp>
#include "Private/pmx/yjw_pmx_model.h"
#include "Private/gltf/yjw_gltf_model.h"

namespace yjw
{
    std::unique_ptr<ModelBuilder> Model::load(std::string filePath, std::string fileName, ModelFileFormat format)
    {
        std::unique_ptr<ModelBuilder> builder = std::make_unique<ModelBuilder>();
        builder->build(filePath, fileName, format);
        return builder;
    }

    int GPUModel::GetEntityCount()
    {
        return m_entities.size();
    }
    GPUModel::GPUMesh* GPUModel::GetGPUMesh(int entity_id)
    {
        if (entity_id<0 || entity_id>=GetEntityCount())
        {
            return nullptr;
        }
        return &m_meshes[m_entities[entity_id].mesh_id];
    }
    Material* GPUModel::GetMaterial(int entity_id)
    {
        if (entity_id < 0 || entity_id >= GetEntityCount())
        {
            return nullptr;
        }
        return &m_material[m_entities[entity_id].material_id];
    }

    void ModelBuilder::build(std::string filePath, std::string fileName, ModelFileFormat format)
    {
        model = std::make_shared<Model>();
        if (format == model_file_format_pmx)
        {
            PMXModelBuilder builder;
            model->m_cpu_model = builder.Build(filePath, fileName);
        }
        else if (format == model_file_format_gltf)
        {
            GLTFModelBuilder builder;
            model->m_cpu_model = builder.Build(filePath, fileName);
        }
        model->m_gpu_model = GenerateCPUModelToGPUModel(model->m_cpu_model);
    }

    std::shared_ptr<GPUModel> ModelBuilder::GenerateCPUModelToGPUModel(std::shared_ptr<CPUModel> cpu_model)
    {
        std::shared_ptr<GPUModel> gpu_model = std::make_shared<GPUModel>();
        for (int id = 0; id < cpu_model->m_pool_buffers.size(); id++)
        {
            CPUModel::CPUBuffer& cpu_buffer = cpu_model->m_pool_buffers[id];
            rpi::RPIBuffer gpu_buffer = rpi::RPICreateGpuVertexBuffer(cpu_buffer.buffer.size());
            rpi::RPIUpdateBuffer(gpu_buffer, cpu_buffer.buffer.data(), 0, cpu_buffer.buffer.size());
            gpu_model->m_pool_buffers.push_back(gpu_buffer);
        }
        for (int id = 0; id < cpu_model->m_pool_buffer_views.size(); id++)
        {
            CPUModel::CPUBufferView& cpu_buffer_view = cpu_model->m_pool_buffer_views[id];
            rpi::RPIBuffer gpu_buffer_view = rpi::RPICreateBufferView(gpu_model->m_pool_buffers[cpu_buffer_view.buffer_id], cpu_buffer_view.offset, cpu_buffer_view.width);
            gpu_model->m_pool_buffer_views.push_back(gpu_buffer_view);
        }
        for (int id = 0; id < cpu_model->m_pool_textures.size(); id++)
        {
            GPUModel::Texture texture{};
            texture.texture = cpu_model->m_pool_textures[id].texture;
            texture.texture_name = cpu_model->m_pool_textures[id].texture_name;
            gpu_model->m_pool_textures.push_back(texture);
        }
        for (int id = 0; id < cpu_model->m_meshes.size(); id++)
        {
            CPUModel::CPUMesh& cpu_mesh = cpu_model->m_meshes[id];
            gpu_model->m_meshes.push_back(GPUModel::GPUMesh{});
            gpu_model->m_meshes.back().first_index = cpu_mesh.first_index;
            gpu_model->m_meshes.back().index_count = cpu_mesh.index_count;
            gpu_model->m_meshes.back().index_buffer = gpu_model->m_pool_buffer_views[cpu_mesh.index_buffer];
            gpu_model->m_meshes.back().is_indices_16bit = cpu_mesh.is_indices_16bit;
            for (CPUModel::VertexBuffer& cpu_vertex_buffer : cpu_mesh.vertex_buffers)
            {
                gpu_model->m_meshes.back().vertex_buffers.push_back(GPUModel::VertexBuffer{});
                gpu_model->m_meshes.back().vertex_buffers.back().type = cpu_vertex_buffer.type;
                gpu_model->m_meshes.back().vertex_buffers.back().buffer = gpu_model->m_pool_buffer_views[cpu_vertex_buffer.buffer];
            }
        }
        for (int id = 0; id < cpu_model->m_material.size(); id++)
        {
            gpu_model->m_material.push_back(cpu_model->m_material[id]);
        }

        for (int id = 0; id < cpu_model->m_entities.size(); id++)
        {
            GPUModel::Entity gpu_entity{};
            gpu_entity.mesh_id = cpu_model->m_entities[id].mesh_id;
            gpu_entity.material_id = cpu_model->m_entities[id].material_id;
            gpu_model->m_entities.push_back(gpu_entity);
        }

        return gpu_model;
    }

}