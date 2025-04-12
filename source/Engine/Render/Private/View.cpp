#include "Engine/Render/Private/View.h"

namespace yjw
{
    RViewProxy::RViewProxy(void* window_handle)
    {
        m_view = new RView(window_handle);
    }

    RViewProxy::~RViewProxy()
    {
        delete m_view;
    }

    void RViewProxy::AttachScene(SceneInterface* scene)
    {
        m_view->AttachScene(scene);
    }

    void RViewProxy::AttachUI(class rhi::ImGuiUI* ui)
    {
        m_view->AttachUI(ui);
    }

    RView* RViewProxy::GetView()
    {
        return m_view;
    }

    RView::RView(void* window_handle)
    {
        m_rpi_window = rpi::RPICreateWindow(window_handle);
    }

    void RView::AttachScene(SceneInterface* scene)
    {
        m_attach_scene = (Scene*)scene;
    }

    void RView::AttachUI(class rhi::ImGuiUI* ui)
    {
        m_attach_ui = ui;
    }

    Scene* RView::GetScene()
    {
        return m_attach_scene;
    }

    rhi::ImGuiUI* RView::GetUI()
    {
        return m_attach_ui;
    }

    rpi::RPIRenderPass RView::GetRenderPass()
    {
        return m_rpi_window.swapchain->GetCurrentRenderPass();
    }

    rpi::RPIWindow RView::GetWindow()
    {
        return m_rpi_window;
    }

    void RView::Present(rpi::RPIContext context)
    {
        rpi::RPIPresent(context, m_rpi_window);
    }
}