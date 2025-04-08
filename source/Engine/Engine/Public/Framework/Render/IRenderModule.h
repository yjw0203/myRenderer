#pragma once
#include "Engine/Render/Private/Scene.h"
#include "Engine/Engine/Public/Module/IModule.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"
#include "Engine/Engine/Public/Framework/Render/Entity.h"

namespace yjw
{
    class IRenderModule : public IModule
    {
    public:
        virtual SceneInterface* CreateScene() = 0;

        virtual void AttachScene(SceneInterface* scene) = 0;
        virtual void AttachUI(class rhi::ImGuiUI* ui) = 0;

        virtual MaterialHandle LoadMaterial(const char* url) = 0;
        virtual void UnloadMaterial(MaterialHandle handle) = 0;

        virtual MeshHandle LoadMesh(const char* url) = 0;
        virtual void UnloadMesh(MeshHandle handle) = 0;
    };

    template<>
    IRenderModule* GetModule();
}