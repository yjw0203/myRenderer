#pragma once
#include "Engine/Utils/Public/Define.h"
#include "Engine/Asset/Public/Asset.h"
#include "Engine/Engine/Public/Asset/Material.h"
#include <vector>
#include "Engine/Math/Public/Math.h"

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

class Meta() SubMeshInfo : public MObject
{
    GENERATED_BODY();
public:
    Property()
    int m_start_index;
    Property()
    int m_index_count;
    Property()
    std::string m_material_slot;
};

class Meta() MeshVertexBuffer : public MObject
{
    GENERATED_BODY();
public:
    Property()
    MeshVertexType m_type;
    Property()
    std::vector<std::uint8_t> m_vertexes;
};

class Meta() MeshAST : public MObject
{
    GENERATED_BODY();
public:
    Property()
    std::vector<SubMeshInfo> m_sub_meshes;
    Property()
    std::vector<MeshVertexBuffer> m_vertex_buffers;
    Property()
    std::vector<std::uint8_t> m_index_buffer;
    Property()
    std::map<std::string, AssetReferece<MaterialInstanceAST>> m_materials;
    Property()
    bool m_is_index_short;
};
 