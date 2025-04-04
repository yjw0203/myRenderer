#pragma once
#include "Engine/Render/Private/Scene.h"
#include "Engine/Engine/Public/Module/IModule.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"

namespace yjw
{
    class IRenderModule : public IModule
    {
    public:
        virtual void AttachScene(Scene* scene) = 0;
        virtual void AttachUI(class rhi::ImGuiUI* ui) = 0;
    };

    template<>
    IRenderModule* CreateModule();
}