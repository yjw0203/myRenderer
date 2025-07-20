#pragma once
#include "Engine/Utils/Public/Define.h"
#include "Engine/Asset/Public/Asset.h"
#include "Engine/Engine/Public/Asset/Material.h"
#include <vector>
#include "Engine/Math/Public/Math.h"

namespace yjw
{
    enum MeshVertexType
    {
        UNKNOW,
        INDEX, // not a attribute
        POSITION,
        NORMAL,
        TANGENT,
        UV0,
        UV1,
        BLEND_INDICES,
        BLEND_WEIGHTS,
        BLEND_INDICES1,
        BLEND_WEIGHTS1,
        BLEND_TYPE,
        COLOR
    };

    template<typename T>
    void to_json(json& j, const MeshVertexType& obj) {
        j = static_cast<int>(obj);
    }

    template<typename T>
    void from_json(const json& j, MeshVertexType& obj) {
        if (j.is_number_integer())
        {
            obj = static_cast<MeshVertexType>(j.get<int>());
        }
    }

    Class(SubMeshInfo)
    {
    public:
        int m_start_index;
        int m_index_count;
        std::string m_material_slot;
    };

    Class(MeshVertexBuffer)
    {
    public:
        MeshVertexType m_type;
        std::vector<std::uint8_t> m_vertexes;
    };

    CAsset(MeshAST)
    {
    public:
        std::vector<SubMeshInfo> m_sub_meshes;
        std::vector<MeshVertexBuffer> m_vertex_buffers;
        std::vector<std::uint8_t> m_index_buffer;
        std::map<std::string, AssetReferece<MaterialInstanceAST>> m_materials;
        bool m_is_index_short;
    };
    
    void to_json(json& j, const yjw::Transform& obj);
    void from_json(const json& j, yjw::Transform& obj);
}

namespace glm
{
    void to_json(json& j, const yjw::Vector3& obj);
    void from_json(const json& j, yjw::Vector3& obj);
    void to_json(json& j, const yjw::Quaternion& obj);
    void from_json(const json& j, yjw::Quaternion& obj);
    void to_json(json& j, const yjw::Matrix4x4& obj);
    void from_json(const json& j, yjw::Matrix4x4& obj);
}