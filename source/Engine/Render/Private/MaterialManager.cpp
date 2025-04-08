#include "Engine/Render/Private/MaterialManager.h"

namespace yjw
{
    MaterialHandle MaterialManager::LoadMaterial(const std::string& url)
    {
        if (m_material_url_to_handle.find(url) != m_material_url_to_handle.end())
        {
            MaterialHandle handle = m_material_url_to_handle[url];
            if (m_material_instances[handle])
            {
                return handle;
            }
        }
        MaterialHandle id = ++m_material_id_allocator;
        m_material_url_to_handle[url] = id;
        m_material_handle_to_url[id] = url;

        MaterialInstance* new_instance = new MaterialInstance();
        new_instance->Build(url.c_str());
        m_material_instances[id] = new_instance;
        return id;
    }

    void MaterialManager::UnloadMaterial(MaterialHandle handle)
    {
        if (m_material_handle_to_url.find(handle) != m_material_handle_to_url.end())
        {
            delete m_material_instances[handle];
            std::string url = m_material_handle_to_url[handle];
            m_material_url_to_handle.erase(url);
            m_material_instances.erase(handle);
            m_material_handle_to_url.erase(handle);
        }
    }

    MaterialInstance* MaterialManager::GetMaterialInstance(MaterialHandle handle)
    {
        if (m_material_instances.find(handle) != m_material_instances.end())
        {
            return m_material_instances[handle];
        }
        return nullptr;
    }
}