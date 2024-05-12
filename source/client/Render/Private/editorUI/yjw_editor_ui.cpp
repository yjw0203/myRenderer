#include "yjw_editor_ui.h"

namespace yjw
{
    void EditorUI::DecodeUI()
    {
        static ImGuiWindowFlags flags = 0;//ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(50, 50));
        ImGui::SetNextWindowSize(ImVec2(250,250));
        static bool wndOpen = true;
        if (ImGui::Begin("Fullscreen", &wndOpen, flags))
        {
            ImGui::Separator();
            ImGui::SliderFloat("Metallic", &m_metallic, 0.0f, 1.0f);
            ImGui::SliderFloat("Roughness", &m_roughness, 0.0f, 1.0f);
            ImGui::End();
        }
    }
}