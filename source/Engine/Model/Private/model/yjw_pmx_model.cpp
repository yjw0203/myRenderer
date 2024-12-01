#include "yjw_pmx_model.h"
#include "Engine/Model/Private/3rd/saba/PMXFile.h"
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

        saba::PMXFile pmxModel;
        ReadPMXFile(&pmxModel, fileHoleName.c_str());

        int indices_buffer_view_id = AddBufferView(AddBuffer(pmxModel.m_faces.data(), pmxModel.m_faces.size() * sizeof(saba::PMXFace)));
        int positon_buffer_view_id = AddBufferView(AddBuffer(pmxModel.m_packed_vertex_data.m_position.data(), pmxModel.m_packed_vertex_data.m_position.size() * sizeof(glm::vec3)));
        int normal_buffer_view_id = AddBufferView(AddBuffer(pmxModel.m_packed_vertex_data.m_normal.data(), pmxModel.m_packed_vertex_data.m_normal.size() * sizeof(glm::vec3)));
        int uv_buffer_view_id = AddBufferView(AddBuffer(pmxModel.m_packed_vertex_data.m_uv.data(), pmxModel.m_packed_vertex_data.m_uv.size() * sizeof(glm::vec2)));
        int blend_indices_buffer_view_id = AddBufferView(AddBuffer(pmxModel.m_packed_vertex_data.m_boneIndices.data(), pmxModel.m_packed_vertex_data.m_boneIndices.size() * sizeof(glm::ivec4)));
        int blend_weights_buffer_view_id = AddBufferView(AddBuffer(pmxModel.m_packed_vertex_data.m_boneWeights.data(), pmxModel.m_packed_vertex_data.m_boneWeights.size() * sizeof(glm::vec4)));
        int blend_type_buffer_view_id = AddBufferView(AddBuffer(pmxModel.m_packed_vertex_data.m_weightType.data(), pmxModel.m_packed_vertex_data.m_weightType.size() * sizeof(int)));

        uint32_t currentOffset = 0;
        for (int i = 0; i < pmxModel.m_materials.size(); i++)
        {
            int mesh_id = AddMesh();
            AddIndexBuffer(mesh_id, indices_buffer_view_id, currentOffset, (uint32_t)pmxModel.m_materials[i].m_numFaceVertices);
            AddVertexBuffer(mesh_id, VertexAttributeType::position, positon_buffer_view_id);
            AddVertexBuffer(mesh_id, VertexAttributeType::normal, normal_buffer_view_id);
            AddVertexBuffer(mesh_id, VertexAttributeType::uv0, uv_buffer_view_id);
            AddVertexBuffer(mesh_id, VertexAttributeType::blend_indices, blend_indices_buffer_view_id);
            AddVertexBuffer(mesh_id, VertexAttributeType::blend_weights, blend_weights_buffer_view_id);
            AddVertexBuffer(mesh_id, VertexAttributeType::blend_type, blend_type_buffer_view_id);

            std::string texPath8 = filePath + "/" + pmxModel.m_textures[pmxModel.m_materials[i].m_textureIndex].m_textureName;;
            
            MaterialInstance* material = new MaterialInstance(&g_pbr_material);
            material->SetTexture("albedoTex", AddTexture(texPath8));
            material->SetDataVec2("metallic_roughness", glm::vec2(0.5, 0.5));
            int material_id = AddMaterial(material);
            AddEntity(mesh_id, material_id);

            currentOffset += pmxModel.m_materials[i].m_numFaceVertices;
        }

        for (int i = 0; i < pmxModel.m_bones.size(); i++)
        {
            BoneData bone;
            bone.name = pmxModel.m_bones[i].m_name;
            bone.index = i;
            bone.parent_index = pmxModel.m_bones[i].m_parentBoneIndex;
            if (pmxModel.m_bones[i].m_parentBoneIndex != -1)
            {
                const auto& parentBone = pmxModel.m_bones[pmxModel.m_bones[i].m_parentBoneIndex];
                bone.binding_pose.m_location = pmxModel.m_bones[i].m_position - parentBone.m_position;
            }
            else
            {
                bone.binding_pose.m_location = pmxModel.m_bones[i].m_position;
            }
            AddBone(bone);
        }
    }
}