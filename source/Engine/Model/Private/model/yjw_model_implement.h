#pragma once
#include "Public/Model/yjw_model.h"

namespace yjw
{
    class ModelBuildImplement
    {
    public:
        virtual std::shared_ptr<CPUModel> Build(const std::string& filePath, const std::string& fileName);
        virtual void FillModelData(const std::string& filePath, const std::string& fileName) = 0;

        //help interface
        int AddBuffer(void* data, int width);
        int AddBufferView(int buffer_id);
        int AddBufferView(int buffer_id, int offset, int width);
        rpi::RPITexture AddTexture(const std::string& texture_name);
        rpi::RPITexture AddTexture(const std::string& texture_name,int width,int height, void* data, int size);
        rpi::RPITexture GetTexture(int id);
        int AddMesh();
        void AddIndexBuffer(int mesh_id, int buffer_view_id,int first_index,int index_count, bool is_indices_16bit = false);
        void AddVertexBuffer(int mesh_id, VertexAttributeType type, int buffer_view_id);
        int AddMaterial(MaterialInstance* material);
        int AddEntity(int mesh_id, int material_id);
        void AddBone(const BoneData& bone);
        const BoneData& GetBone(int index);
    public:
        std::shared_ptr<CPUModel> m_cpu_model;
    };
}