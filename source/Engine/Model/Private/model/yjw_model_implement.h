#pragma once
#include "Engine/Model/Public/yjw_model.h"
#include "Engine/Engine/Public/Asset/Mesh.h"

namespace yjw
{
    class ModelBuildImplement
    {
    public:
        virtual std::shared_ptr<CPUModel> Build(const std::string& filePath, const std::string& fileName);
        virtual void FillModelData(const std::string& filePath, const std::string& fileName) = 0;

        //help interface
        int AddBuffer(MeshVertexType type, void* data, int width);
        int AddBufferView(int buffer_id);
        int AddBufferView(int buffer_id, int offset, int width);
        int GetBufferIDFromBufferView(int buffer_view_id);
        void SetBufferType(int buffer_id, MeshVertexType type);
        int AddTexture(const std::string& texture_name);
        int AddTexture(const std::string& texture_name,int width,int height, void* data, int size);
        int AddMesh();
        void AddIndexBuffer(int mesh_id, int buffer_view_id,int first_index,int index_count, bool is_indices_16bit = false);
        void AddVertexBuffer(int mesh_id, MeshVertexType type, int buffer_view_id);
        int AddMaterial(const CPUModel::Material& material);
        int AddMaterialInstance(const CPUModel::MaterialIns& material_instance);
        int AddEntity(int mesh_id, int material_ins_id);
        void AddBone(const BoneData& bone);
        const BoneData& GetBone(int index);
    public:
        std::shared_ptr<CPUModel> m_cpu_model;
    };
}