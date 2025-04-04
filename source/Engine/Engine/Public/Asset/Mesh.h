#pragma once
#include "Engine/Utils/Public/Define.h"
#include "Engine/Asset/Public/Asset.h"
#include "Engine/Engine/Public/Asset/Material.h"
#include <vector>

namespace yjw
{
    enum MeshVertexType
    {
        UNKNOW,
        INDEX, // not a attribute
        POSITION,
        NORMAL,
        TARGENT,
        UV0,
        UV1,
        BLEND_INDICES,
        BLEND_WEIGHTS,
        BLEND_TYPE
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

    class RawBuffer : public std::vector<std::uint8_t>
    {
        using std::vector<std::uint8_t>::vector;
    };

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
        RawBuffer m_vertexes;
    };

    CAsset(MeshAST)
    {
    public:
        std::vector<SubMeshInfo> m_sub_meshes;
        std::vector<MeshVertexBuffer> m_vertex_buffers;
        RawBuffer m_index_buffer;
        std::map<std::string, AssetReferece<MaterialInstanceAST>> m_materials;
        bool m_is_index_short;
    };
    

    void to_json(json& j, const yjw::RawBuffer& obj);
    void from_json(const json& j, yjw::RawBuffer& obj);
}
