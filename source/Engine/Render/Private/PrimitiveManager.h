#pragma once
#include "Engine/Render/Private/Primitive.h"
#include "Engine/Render/Public/RenderAPI.h"
#include <set>

namespace yjw
{
    class PrimitiveManager
    {
    public:
        RdGeometryPtr LoadMesh(const std::string& url);
        void UnloadMesh(RdGeometryPtr handle);
        RdGeometry* GetMesh(RdGeometryPtr handle);
    private:
        std::set<RdGeometry*> m_meshes;
        std::unordered_map<std::string, RdGeometryPtr> m_mesh_url_to_handle;
        std::unordered_map<RdGeometryPtr, std::string> m_mesh_handle_to_url;
    };
}