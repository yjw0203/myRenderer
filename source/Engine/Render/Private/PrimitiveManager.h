#pragma once
#include "Engine/Engine/Public/Framework/Render/Entity.h"
#include "Engine/Render/Private/Primitive.h"

namespace yjw
{
    class PrimitiveManager
    {
    public:
        MeshHandle LoadMesh(const std::string& url);
        void UnloadMesh(MeshHandle handle);
        Primitive* GetMesh(MeshHandle handle);
    private:
        MeshHandle m_mesh_id_allocator{ 0 };
        std::unordered_map<MeshHandle, Primitive*> m_meshes;
        std::unordered_map<std::string, MeshHandle> m_mesh_url_to_handle;
        std::unordered_map<MeshHandle, std::string> m_mesh_handle_to_url;
    };
}