#include "RHI/externs/imgui/yjw_rhi_imgui_window.h"

namespace yjw
{
    class EditorUI : public rhi::ImGuiUI
    {
    public:
        virtual void DecodeUI() override;

    public:
        float m_metallic = 0.5;
        float m_roughness = 0.5;
    };
}