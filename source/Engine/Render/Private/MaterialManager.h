#pragma once
#include "Engine/Render/Private/Material.h"
#include "Engine/Render/Public/RenderAPI.h"
#include <set>
namespace yjw
{
    class MaterialManager
    {
    public:
        RdMaterialPtr LoadMaterial(const std::string& url);
        void UnloadMaterial(RdMaterialPtr handle);
        RdMaterial* GetMaterialInstance(RdMaterialPtr handle);
    private:
        std::set<RdMaterial*> m_material_instances;
        std::unordered_map<std::string, RdMaterial*> m_material_url_to_handle;
        std::unordered_map<RdMaterial*, std::string> m_material_handle_to_url;
    };
}