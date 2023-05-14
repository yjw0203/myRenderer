#include "yjw_mesh.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "client/Model/PMX/yjw_pmx_helper.h"
#include "client/Base/yjw_unicode_util.h"
#include "generate/projectInfo.h"
#include <glm/glm.hpp>
namespace yjw
{
    std::unique_ptr<MeshBuilder> Mesh::load(std::string filePath, std::string fileName)
    {
        std::unique_ptr<MeshBuilder> builder = std::make_unique<MeshBuilder>();
        builder->build(filePath, fileName);
        return builder;
    }

    void MeshBuilder::build(std::string filePath, std::string fileName)
    {
        fileName = filePath + "/" + fileName;
        PMXModel pmxModel = PMXModel::load(fileName.c_str());

        mesh.indices.swap(pmxModel.m_indices);
        mesh.vertices.resize(pmxModel.m_vertices.size());
        for (int i = 0; i < pmxModel.m_vertices.size(); i++)
        {
            mesh.vertices[i].pos = pmxModel.m_vertices[i].m_position;
            mesh.vertices[i].normal = pmxModel.m_vertices[i].m_normal;
            mesh.vertices[i].uv = pmxModel.m_vertices[i].m_uv;
        }

        mesh.materials.resize(pmxModel.m_materials.size());
        uint32_t currentOffset = 0;
        for (int i = 0; i < pmxModel.m_materials.size(); i++)
        {
            mesh.materials[i].offset = currentOffset;
            mesh.materials[i].size = pmxModel.m_materials[i].m_numFaceVertices;
            std::string texPath8 = filePath + "/" + pmxModel.m_textures[pmxModel.m_materials[i].m_textureIndex].m_textureName;
            mesh.materials[i].texture = new rhi::RHITexture2DFromFile(texPath8.c_str());
            mesh.materials[i].textureView = new rhi::RHIResourceView(rhi::ResourceViewType::srv, mesh.materials[i].texture, rhi::RHIFormat::R8G8B8A8_srgb);
            currentOffset += mesh.materials[i].size;
        }

    }

}