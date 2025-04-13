#pragma once
#include "Engine/Engine/Public/Module/IModule.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"
#include "Engine/Engine/Public/Framework/Render/Entity.h"
#include "Engine/Engine/Public/Framework/Render/SceneInterface.h"
#include "Engine/Engine/Public/Framework/Render/RenderHit.h"
#include <vector>

namespace yjw
{
    class IRenderModule : public IModule
    {
    public:
        virtual SceneInterface* CreateScene() = 0;

        virtual MaterialHandle LoadMaterial(const char* url) = 0;
        virtual void UnloadMaterial(MaterialHandle handle) = 0;

        virtual MeshHandle LoadMesh(const char* url) = 0;
        virtual void UnloadMesh(MeshHandle handle) = 0;

        virtual ViewProxy* CreateView(void* window) = 0;
        virtual void Destroy(ViewProxy* view) = 0;

        virtual void AttachView(ViewProxy* view) = 0;
        virtual void DettachView(ViewProxy* view) = 0;

        virtual void AddPendingHitRequest(const char* group_name, const RenderHitRequest& request) = 0;
        virtual void GetProcessedHitRequest(const char* group_name, std::vector<RenderHitRequest>& proccessed_request) = 0;
    };

    template<>
    IRenderModule* GetModule();
}