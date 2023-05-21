#include "yjw_model.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "client/Model/PMX/yjw_pmx_helper.h"
#include "client/Base/yjw_unicode_util.h"
#include "generate/projectInfo.h"

#include <glm/glm.hpp>
namespace yjw
{
    std::unique_ptr<ModelBuilder> Model::load(std::string filePath, std::string fileName)
    {
        std::unique_ptr<ModelBuilder> builder = std::make_unique<ModelBuilder>();
        builder->build(filePath, fileName);
        return builder;
    }

    void ModelBuilder::build(std::string filePath, std::string fileName)
    {
        model = std::make_shared<Model>();

        fileName = filePath + "/" + fileName;
        PMXModel pmxModel = PMXModel::load(fileName.c_str());

        model->mesh = std::make_shared<Mesh>();
        model->mesh->indices.swap(pmxModel.m_indices);
        model->mesh->vertices.resize(pmxModel.m_vertices.size());
        for (int i = 0; i < pmxModel.m_vertices.size(); i++)
        {
            model->mesh->vertices[i].pos = pmxModel.m_vertices[i].m_position;
            model->mesh->vertices[i].normal = pmxModel.m_vertices[i].m_normal;
            model->mesh->vertices[i].uv = pmxModel.m_vertices[i].m_uv;
        }

        model->mesh->vertex_buffer = std::make_unique<rhi::RHIBuffer>(sizeof(MeshVertex) * model->mesh->vertices.size(), rhi::RHIResourceUsageBits::allow_vertex_buffer, rhi::RHIMemoryType::default_);
        model->mesh->index_buffer = std::make_unique<rhi::RHIBuffer>(sizeof(uint32_t) * model->mesh->indices.size(), rhi::RHIResourceUsageBits::allow_index_buffer, rhi::RHIMemoryType::default_);

        rhi::IRHI::Get()->writeResourceImmidiately(model->mesh->vertex_buffer.get(), model->mesh->vertices.data(), sizeof(MeshVertex) * model->mesh->vertices.size(),0);
        rhi::IRHI::Get()->writeResourceImmidiately(model->mesh->index_buffer.get(), model->mesh->indices.data(), sizeof(uint32_t) * model->mesh->indices.size(),0);

        model->materials.resize(pmxModel.m_materials.size());
        uint32_t currentOffset = 0;
        for (int i = 0; i < pmxModel.m_materials.size(); i++)
        {
            model->materials[i] = std::make_shared<Material>();
            model->mesh->subMeshes.push_back(Mesh::SubMesh{ currentOffset ,(uint32_t)pmxModel.m_materials[i].m_numFaceVertices });
            std::string texPath8 = filePath + "/" + pmxModel.m_textures[pmxModel.m_materials[i].m_textureIndex].m_textureName;
            model->materials[i]->texture = new rhi::RHITexture2DFromFile(texPath8.c_str());
            model->materials[i]->textureShaderResource = std::make_shared<rhi::RHIShaderResourceTexture>(model->materials[i]->texture, rhi::RHIFormat::R8G8B8A8_srgb);

            model->entitys.push_back(Entity{ model->mesh ,(uint32_t)i,model->materials[i] });
            currentOffset += pmxModel.m_materials[i].m_numFaceVertices;
        }

    }

}