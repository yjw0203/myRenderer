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
        POSITION,
        NORMAL,
        TARGENT,
        UV0,
        UV1,
        BLEND_WEIGHTS,
        BLEND_TYPE
    };

    template<typename T>
    void to_json(json& j, const MeshVertexType& obj) {
        j = static_cast<int>(obj);
    }

    template<typename T>
    void from_json(const json& j, MeshVertexType& obj) {
        if (j.is_number_integer)
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
        std::vector<char> m_vertexes;
    };

    CAsset(MeshAST)
    {
    public:
        std::vector<SubMeshInfo> m_sub_meshes;
        std::vector<MeshVertexBuffer> m_vertex_buffers;
        std::vector<char> m_index_buffer;
        std::map<std::string, AssetReferece<MaterialInstanceAST>> m_materials;
        bool m_is_index_short;
    };
}