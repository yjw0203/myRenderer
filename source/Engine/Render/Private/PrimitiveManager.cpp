#include "Engine/Render/Private/PrimitiveManager.h"

namespace yjw
{
    MeshHandle PrimitiveManager::LoadMesh(const std::string& url)
    {
        if (m_mesh_url_to_handle.find(url) != m_mesh_url_to_handle.end())
        {
            MeshHandle handle = m_mesh_url_to_handle[url];
            if (m_meshes[handle])
            {
                return handle;
            }
        }
        MeshHandle id = ++m_mesh_id_allocator;
        m_mesh_url_to_handle[url] = id;
        m_mesh_handle_to_url[id] = url;

        Primitive* new_instance = new Primitive();
        new_instance->Build(url.c_str());
        m_meshes[id] = new_instance;
        return id;
    }

    void PrimitiveManager::UnloadMesh(MeshHandle handle)
    {
        if (m_mesh_handle_to_url.find(handle) != m_mesh_handle_to_url.end())
        {
            delete m_meshes[handle];
            std::string url = m_mesh_handle_to_url[handle];
            m_mesh_url_to_handle.erase(url);
            m_meshes.erase(handle);
            m_mesh_handle_to_url.erase(handle);
        }
    }

    Primitive* PrimitiveManager::GetMesh(MeshHandle handle)
    {
        if (m_meshes.find(handle) != m_meshes.end())
        {
            return m_meshes[handle];
        }
        return nullptr;
    }
}