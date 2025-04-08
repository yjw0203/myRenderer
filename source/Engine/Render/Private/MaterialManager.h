#pragma once
#include "Engine/Engine/Public/Framework/Render/Entity.h"
#include "Engine/Render/Private/Material.h"

namespace yjw
{
    class MaterialManager
    {
    public:
        MaterialHandle LoadMaterial(const std::string& url);
        void UnloadMaterial(MaterialHandle handle);
        MaterialInstance* GetMaterialInstance(MaterialHandle handle);
    private:
        MaterialHandle m_material_id_allocator{0};
        std::unordered_map<MaterialHandle, MaterialInstance*> m_material_instances;
        std::unordered_map<std::string, MaterialHandle> m_material_url_to_handle;
        std::unordered_map<MaterialHandle, std::string> m_material_handle_to_url;
    };
}