#pragma once
#include "Engine/Engine/Public/Module/IRenderModule.h"

namespace yjw
{
    class Scene;
    class RenderSystem;
    class RenderModule : public IRenderModule
    {
    public:
        virtual void Startup();
        virtual void Shutdown();
        virtual void AttachScene(Scene* scene) override;
        virtual void AttachUI(rhi::ImGuiUI* ui) override;
        virtual void Tick(float deltaTime);

    private:
        RenderSystem* m_render_system{};
    };

}