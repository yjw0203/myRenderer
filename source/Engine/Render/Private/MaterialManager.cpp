#include "Engine/Render/Private/MaterialManager.h"

RdMaterialPtr MaterialManager::LoadMaterial(const std::string& url)
{
    if (m_material_url_to_handle.find(url) != m_material_url_to_handle.end())
    {
        RdMaterialPtr handle = m_material_url_to_handle[url];
        if (handle)
        {
            return handle;
        }
    }
    RdMaterialPtr material = new RdMaterial();
    m_material_url_to_handle[url] = material;
    m_material_handle_to_url[material] = url;

    material->Build(url.c_str());
    m_material_instances.insert(material);
    return material;
}

void MaterialManager::UnloadMaterial(RdMaterialPtr handle)
{
    if (m_material_handle_to_url.find(handle) != m_material_handle_to_url.end())
    {
        delete handle;
        std::string url = m_material_handle_to_url[handle];
        m_material_url_to_handle.erase(url);
        m_material_instances.erase(handle);
        m_material_handle_to_url.erase(handle);
    }
}

RdMaterial* MaterialManager::GetMaterialInstance(RdMaterialPtr handle)
{
    if (m_material_instances.find(handle) != m_material_instances.end())
    {
        return handle;
    }
    return nullptr;
}
