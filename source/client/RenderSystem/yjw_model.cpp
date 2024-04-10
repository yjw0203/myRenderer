#include "yjw_model.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "client/Model/PMX/yjw_pmx_helper.h"
#include "client/Base/yjw_unicode_util.h"
#include "generate/projectInfo.h"
#include "rhi/rpi/yjw_rpi_header.h"
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
        model->mesh->vertices.pos.resize(pmxModel.m_vertices.size());
        model->mesh->vertices.normal.resize(pmxModel.m_vertices.size());
        model->mesh->vertices.uv.resize(pmxModel.m_vertices.size());
        for (int i = 0; i < pmxModel.m_vertices.size(); i++)
        {
            model->mesh->vertices.pos[i] = pmxModel.m_vertices[i].m_position;
            model->mesh->vertices.normal[i] = pmxModel.m_vertices[i].m_normal;
            model->mesh->vertices.uv[i] = pmxModel.m_vertices[i].m_uv;
        }

        model->mesh->vertex_buffer.pos = rpi::RPICreateGpuVertexBuffer(sizeof(glm::vec3) * model->mesh->vertices.pos.size());
        model->mesh->vertex_buffer.normal = rpi::RPICreateGpuVertexBuffer(sizeof(glm::vec3) * model->mesh->vertices.normal.size());
        model->mesh->vertex_buffer.uv = rpi::RPICreateGpuVertexBuffer(sizeof(glm::vec2) * model->mesh->vertices.uv.size());
        model->mesh->index_buffer = rpi::RPICreateGpuIndexBuffer(sizeof(uint32_t) * model->mesh->indices.size());

        rpi::RPIUpdateBuffer(model->mesh->vertex_buffer.pos, model->mesh->vertices.pos.data(), 0, sizeof(glm::vec3) * model->mesh->vertices.pos.size());
        rpi::RPIUpdateBuffer(model->mesh->vertex_buffer.normal, model->mesh->vertices.normal.data(), 0, sizeof(glm::vec3) * model->mesh->vertices.normal.size());
        rpi::RPIUpdateBuffer(model->mesh->vertex_buffer.uv, model->mesh->vertices.uv.data(), 0, sizeof(glm::vec2) * model->mesh->vertices.uv.size());
        rpi::RPIUpdateBuffer(model->mesh->index_buffer, model->mesh->indices.data(), 0, sizeof(uint32_t) * model->mesh->indices.size());

        model->materials.resize(pmxModel.m_materials.size());
        uint32_t currentOffset = 0;
        for (int i = 0; i < pmxModel.m_materials.size(); i++)
        {
            model->materials[i] = std::make_shared<Material>();
            model->mesh->subMeshes.push_back(Mesh::SubMesh{ currentOffset ,(uint32_t)pmxModel.m_materials[i].m_numFaceVertices });
            std::string texPath8 = filePath + "/" + pmxModel.m_textures[pmxModel.m_materials[i].m_textureIndex].m_textureName;
            model->materials[i]->texture = rpi::RPICreateTexture2DFromFile(texPath8.c_str());
            model->materials[i]->textureShaderResource = rpi::RPICreateTextureView(model->materials[i]->texture, rpi::RPIFormat::R8G8B8A8_srgb);
            model->materials[i]->diffuse = pmxModel.m_materials[i].m_diffuse;
            model->materials[i]->specular = pmxModel.m_materials[i].m_specular;
            model->materials[i]->specularPower = pmxModel.m_materials[i].m_specularPower;
            model->materials[i]->ambient = pmxModel.m_materials[i].m_ambient;

            model->entitys.push_back(Entity{ model->mesh ,(uint32_t)i,model->materials[i] });
            currentOffset += pmxModel.m_materials[i].m_numFaceVertices;
        }

    }

}