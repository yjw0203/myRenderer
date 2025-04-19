#include "Engine/Render/Private/PrimitiveManager.h"

namespace yjw
{
    RdGeometryPtr PrimitiveManager::LoadMesh(const std::string& url)
    {
        if (m_mesh_url_to_handle.find(url) != m_mesh_url_to_handle.end())
        {
            RdGeometryPtr handle = m_mesh_url_to_handle[url];
            if (handle)
            {
                return handle;
            }
        }
        RdGeometryPtr geometry = new RdGeometry();
        m_mesh_url_to_handle[url] = geometry;
        m_mesh_handle_to_url[geometry] = url;

        geometry->Build(url.c_str());
        m_meshes.insert(geometry);
        return geometry;
    }

    void PrimitiveManager::UnloadMesh(RdGeometryPtr handle)
    {
        if (m_mesh_handle_to_url.find(handle) != m_mesh_handle_to_url.end())
        {
            delete handle;
            std::string url = m_mesh_handle_to_url[handle];
            m_mesh_url_to_handle.erase(url);
            m_meshes.erase(handle);
            m_mesh_handle_to_url.erase(handle);
        }
    }

    RdGeometry* PrimitiveManager::GetMesh(RdGeometryPtr handle)
    {
        if (m_meshes.find(handle) != m_meshes.end())
        {
            return handle;
        }
        return nullptr;
    }
}