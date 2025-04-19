#include "Engine/Render/Private/View.h"
#include "Engine/Render/Private/Pass/PickPass.h"
#include "Engine/Render/Private/Renderer/ForwardRenderer.h"

namespace yjw
{
    RdView::RdView(void* window_handle)
    {
        m_rpi_window = rpi::RPICreateWindow(window_handle);

        m_renderer = new ForwardRenderer();
        m_renderer->Initialize();
        m_renderer->SetView(this);

        m_pick_pass = new PickPass();
        m_pick_pass->Initialize();
        m_pick_pass->AttachView(this);
    }

    void RdView::Draw()
    {
        m_attach_scene->Update();
        m_pick_pass->Submit();
        m_renderer->Render();
    }

    void RdView::AttachScene(RdScene* scene)
    {
        m_attach_scene = (RdScene*)scene;
    }

    void RdView::AttachUI(class rhi::ImGuiUI* ui)
    {
        m_attach_ui = ui;
    }

    RdScene* RdView::GetScene()
    {
        return m_attach_scene;
    }

    rhi::ImGuiUI* RdView::GetUI()
    {
        return m_attach_ui;
    }

    rpi::RPIRenderPass RdView::GetRenderPass()
    {
        return m_rpi_window.swapchain->GetCurrentRenderPass();
    }

    rpi::RPIWindow RdView::GetWindow()
    {
        return m_rpi_window;
    }

    void RdView::AddPendingHitRequest(const char* group_name, const RdHitRequestStruct& request)
    {
        m_pick_pass->AddPendingHitRequest(group_name, request);
    }

    void RdView::GetProcessedHitRequest(const char* group_name, std::vector<RdHitRequestStruct>& proccessed_request)
    {
        m_pick_pass->GetProcessedHitRequest(group_name, proccessed_request);
    }
}