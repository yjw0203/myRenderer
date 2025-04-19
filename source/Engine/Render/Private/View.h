#pragma once
#include "Engine/Render/Private/Scene.h"

namespace yjw
{
    class RdView
    {
    public:
        RdView(void* window_handle);
        void Draw();
        void AttachScene(RdScene* scene);
        void AttachUI(class rhi::ImGuiUI* ui);
        RdScene* GetScene();
        class rhi::ImGuiUI* GetUI();
        rpi::RPIRenderPass GetRenderPass();
        rpi::RPIWindow GetWindow();

        void AddPendingHitRequest(const char* group_name, const RdHitRequestStruct& request);
        void GetProcessedHitRequest(const char* group_name, std::vector<RdHitRequestStruct>& proccessed_request);

    private:
        rpi::RPIWindow m_rpi_window{};
        RdScene* m_attach_scene = nullptr;
        class rhi::ImGuiUI* m_attach_ui = nullptr;

        class ForwardRenderer* m_renderer{};

        class PickPass* m_pick_pass{};
    };
}