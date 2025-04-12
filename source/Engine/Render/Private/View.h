#pragma once
#include "Engine/Engine/Public/Framework/Render/SceneInterface.h"
#include "Engine/Render/Private/Scene.h"

namespace yjw
{
    class RView;
    class RViewProxy : public ViewProxy
    {
    public:
        RViewProxy(void* window_handle);
        ~RViewProxy();
        virtual void AttachScene(SceneInterface* scene) override;
        virtual void AttachUI(class rhi::ImGuiUI* ui) override;
        RView* GetView();
    private:
        RView* m_view = nullptr;
    };

    class RView
    {
    public:
        RView(void* window_handle);
        void AttachScene(SceneInterface* scene);
        void AttachUI(class rhi::ImGuiUI* ui);
        Scene* GetScene();
        class rhi::ImGuiUI* GetUI();
        rpi::RPIRenderPass GetRenderPass();
        rpi::RPIWindow GetWindow();
        void Present(rpi::RPIContext context);
    private:
        rpi::RPIWindow m_rpi_window{};
        Scene* m_attach_scene = nullptr;
        class rhi::ImGuiUI* m_attach_ui = nullptr;
    };
}