#include "yjw_pmx_model.h"
#include "yjw_pmx_helper.h"
#include <memory>

namespace yjw
{
    PMXModelBuilder::~PMXModelBuilder()
    {
        int x = 1;
    }
    void PMXModelBuilder::FillModelData(const std::string& filePath, const std::string& fileName)
    {
        std::string fileHoleName = filePath + "/" + fileName;

        PMXModel pmxModel = PMXModel::load(fileHoleName.c_str());

        int indices_buffer_view_id = AddBufferView(AddBuffer(pmxModel.m_indices.data(), pmxModel.m_indices.size() * sizeof(uint32_t)));
        int positon_buffer_view_id = AddBufferView(AddBuffer(pmxModel.m_vertices.m_position.data(), pmxModel.m_vertices.m_position.size() * sizeof(glm::vec3)));
        int normal_buffer_view_id = AddBufferView(AddBuffer(pmxModel.m_vertices.m_normal.data(), pmxModel.m_vertices.m_normal.size() * sizeof(glm::vec3)));
        int uv_buffer_view_id = AddBufferView(AddBuffer(pmxModel.m_vertices.m_uv.data(), pmxModel.m_vertices.m_uv.size() * sizeof(glm::vec2)));

        uint32_t currentOffset = 0;
        for (int i = 0; i < pmxModel.m_materials.size(); i++)
        {
            int mesh_id = AddMesh();
            AddIndexBuffer(mesh_id, indices_buffer_view_id, currentOffset, (uint32_t)pmxModel.m_materials[i].m_numFaceVertices);
            AddVertexBuffer(mesh_id, VertexAttributeType::position, positon_buffer_view_id);
            AddVertexBuffer(mesh_id, VertexAttributeType::normal, normal_buffer_view_id);
            AddVertexBuffer(mesh_id, VertexAttributeType::uv, uv_buffer_view_id);

            std::string texPath8 = filePath + "/" + pmxModel.m_textures[pmxModel.m_materials[i].m_textureIndex].m_textureName;;
            
            Material material;
            material.textureShaderResource = AddTexture(texPath8);
            material.diffuse = pmxModel.m_materials[i].m_diffuse;
            material.specular = pmxModel.m_materials[i].m_specular;
            material.specularPower = pmxModel.m_materials[i].m_specularPower;
            material.ambient = pmxModel.m_materials[i].m_ambient;
            int material_id = AddMaterial(material);
            AddEntity(mesh_id, material_id);

            currentOffset += pmxModel.m_materials[i].m_numFaceVertices;
        }
    }
}