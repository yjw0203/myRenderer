#include "RHI/externs/imgui/yjw_rhi_imgui_window.h"

namespace yjw
{
    class EditorUI : public rhi::ImGuiUI
    {
    public:
        virtual void DecodeUI() override;

    public:
        void* m_scene_texture = nullptr;
        class Scene* m_scene = nullptr;

    private:
        void ShowMenuBar();
        void ShowWorldObject();
        void ShowMenu();
        void ShowActorList();
        void ShowViewport();
    };
}