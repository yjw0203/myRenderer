#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"
#include "Engine/Math/Public/Transform.h"

namespace yjw
{
    class EditorUI : public rhi::ImGuiUI
    {
    public:
        EditorUI(class World* world) { m_world = world; };
        virtual void DecodeUI() override;
        void SetEditTransformPtr(Transform* transform) { m_edit_transform_ptr = transform; }
    public:
        void* m_scene_texture = nullptr;
        class World* m_world = nullptr;
    private:
        void ShowOption();
        Transform* m_edit_transform_ptr{};
    };
}