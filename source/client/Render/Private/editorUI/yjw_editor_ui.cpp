#include "yjw_editor_ui.h"

namespace yjw
{
    void EditorUI::DecodeUI()
    {
        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        static bool wndOpen = true;
        /*if (ImGui::Begin("Fullscreen", &wndOpen, flags))
        {
            ImGui::Separator();
            static int sampler = 1;
            const char* samplerItems[] = { "PCG" };
            ImGui::Combo("Sampler", &sampler, samplerItems, IM_ARRAYSIZE(samplerItems));
            const char* filterItems[] = { "Tent" };
            ImGui::Combo("Filter", &sampler, filterItems, IM_ARRAYSIZE(filterItems));
            const char* misItems[] = { "BSDF+LIGHT" };
            ImGui::Combo("MIS", &sampler, misItems, IM_ARRAYSIZE(misItems));
            const char* integratorItems[] = { "Path", "AO" };
            ImGui::Combo("Integrator", &sampler, integratorItems, IM_ARRAYSIZE(integratorItems));
            ImGui::DragInt("Sample Count", &sampler, 256, 0, 8192, "%d%", ImGuiSliderFlags_AlwaysClamp);
            ImGui::DragInt("Max Bounce", &sampler, 1, 0, 100, "%d%", ImGuiSliderFlags_AlwaysClamp);
            static float x = 0;
            ImGui::DragFloat("Intensity Clamp", &x, 1.f, 1.0f, 100.0f, "%.1f");
            ImGui::Separator();
            ImGui::End();
        }*/
    }
}