#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_layer.h"

using namespace rpi;
class ImGuiPass
{
public:
    void Initialize();
    void Destroy();
    void Submit(RPIContext context);
    void LoadResource(rhi::ImGuiUI* ui);

private:
    rhi::ImGuiUI* m_ui{};
    RHIImguiLayer* m_imgui_layer{};
};
