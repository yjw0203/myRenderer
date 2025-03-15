#pragma once
#include "Engine/Utils/Public/Define.h"
#include <vector>

namespace yjw
{
    Class(SubMeshInfo)
    {
    public:
        int offset;
        int size;
    };

    CAsset(MeshAST)
    {
    public:

        std::vector<SubMeshInfo> m_sub_meshes;
        std::vector<std::vector<char>> m_vertexes;
        std::vector<char> m_indices;
    };
}