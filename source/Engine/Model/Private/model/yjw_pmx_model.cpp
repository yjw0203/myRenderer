#include "yjw_pmx_model.h"
#include "Engine/Model/Private/3rd/saba/PMXFile.h"
#include <memory>

PMXModelBuilder::~PMXModelBuilder()
{
    int x = 1;
}

void PMXModelBuilder::FillModelData(const std::string& filePath, const std::string& fileName)
{
    std::string fileHoleName = filePath + "/" + fileName;

    saba::PMXFile pmxModel;
    ReadPMXFile(&pmxModel, fileHoleName.c_str());

    int indices_buffer_view_id = AddBufferView(AddBuffer(MeshVertexType::INDEX, pmxModel.m_faces.data(), pmxModel.m_faces.size() * sizeof(saba::PMXFace)));
    int positon_buffer_view_id = AddBufferView(AddBuffer(MeshVertexType::POSITION, pmxModel.m_packed_vertex_data.m_position.data(), pmxModel.m_packed_vertex_data.m_position.size() * sizeof(glm::vec3)));
    int normal_buffer_view_id = AddBufferView(AddBuffer(MeshVertexType::NORMAL, pmxModel.m_packed_vertex_data.m_normal.data(), pmxModel.m_packed_vertex_data.m_normal.size() * sizeof(glm::vec3)));
    int uv_buffer_view_id = AddBufferView(AddBuffer(MeshVertexType::UV0, pmxModel.m_packed_vertex_data.m_uv.data(), pmxModel.m_packed_vertex_data.m_uv.size() * sizeof(glm::vec2)));
    int blend_indices_buffer_view_id = AddBufferView(AddBuffer(MeshVertexType::BLEND_INDICES, pmxModel.m_packed_vertex_data.m_boneIndices.data(), pmxModel.m_packed_vertex_data.m_boneIndices.size() * sizeof(glm::ivec4)));
    int blend_weights_buffer_view_id = AddBufferView(AddBuffer(MeshVertexType::BLEND_WEIGHTS, pmxModel.m_packed_vertex_data.m_boneWeights.data(), pmxModel.m_packed_vertex_data.m_boneWeights.size() * sizeof(glm::vec4)));
    int blend_type_buffer_view_id = AddBufferView(AddBuffer(MeshVertexType::BLEND_TYPE, pmxModel.m_packed_vertex_data.m_weightType.data(), pmxModel.m_packed_vertex_data.m_weightType.size() * sizeof(int)));

    CPUModel::Material material_ast{};
    material_ast.m_shader = "ForwardPBR.hlsl";
    material_ast.m_entry = "PSMain";
    int mat_id = AddMaterial(material_ast);

    uint32_t currentOffset = 0;
    for (int i = 0; i < pmxModel.m_materials.size(); i++)
    {
        int mesh_id = AddMesh();
        AddIndexBuffer(mesh_id, indices_buffer_view_id, currentOffset, (uint32_t)pmxModel.m_materials[i].m_numFaceVertices);
        AddVertexBuffer(mesh_id, MeshVertexType::POSITION, positon_buffer_view_id);
        AddVertexBuffer(mesh_id, MeshVertexType::NORMAL, normal_buffer_view_id);
        AddVertexBuffer(mesh_id, MeshVertexType::UV0, uv_buffer_view_id);
        AddVertexBuffer(mesh_id, MeshVertexType::BLEND_INDICES, blend_indices_buffer_view_id);
        AddVertexBuffer(mesh_id, MeshVertexType::BLEND_WEIGHTS, blend_weights_buffer_view_id);
        AddVertexBuffer(mesh_id, MeshVertexType::BLEND_TYPE, blend_type_buffer_view_id);

        std::string texPath8 = filePath + "/" + pmxModel.m_textures[pmxModel.m_materials[i].m_textureIndex].m_textureName;;
            
        int tex_id = AddTexture(texPath8);

        CPUModel::MaterialIns mat_ins{};
        mat_ins.material_id = mat_id;
        mat_ins.m_float_params["metallic"] = 0.5;
        mat_ins.m_float_params["roughness"] = 0.5;
        mat_ins.m_texture_params["albedoTex"] = tex_id;

        int mat_ins_id = AddMaterialInstance(mat_ins);
        AddEntity(mesh_id, mat_ins_id);

        currentOffset += pmxModel.m_materials[i].m_numFaceVertices;
    }

    for (int i = 0; i < pmxModel.m_bones.size(); i++)
    {
        RawBone bone;
        bone.m_name = pmxModel.m_bones[i].m_name;
        bone.m_index = i;
        bone.m_parent_index = pmxModel.m_bones[i].m_parentBoneIndex;
        if (pmxModel.m_bones[i].m_parentBoneIndex != -1)
        {
            const auto& parentBone = pmxModel.m_bones[pmxModel.m_bones[i].m_parentBoneIndex];
            bone.m_binding_pose.m_location = pmxModel.m_bones[i].m_position - parentBone.m_position;
        }
        else
        {
            bone.m_binding_pose.m_location = pmxModel.m_bones[i].m_position;
        }
        AddBone(bone);
    }
}
