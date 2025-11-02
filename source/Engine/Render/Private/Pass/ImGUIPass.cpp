#include "Engine/Render/Private/Pass/ImGuiPass.h"
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
#include "projectInfo.h"

void ImGuiPass::Initialize()
{
    m_imgui_layer = dynamic_cast<RHIImguiLayer*>(RPIGetLayer(rhi_layer_imgui));
}

void ImGuiPass::LoadResource(rhi::ImGuiUI* ui)
{
    m_ui = ui;
}

void ImGuiPass::Destroy()
{
        
}

void ImGuiPass::Submit(RPIContext context)
{
    if (m_ui)
    {
        RPICmdPushEvent(context, "ImGuiPass");
        m_imgui_layer->NewFrame();
        m_ui->DecodeUI();
        m_imgui_layer->Render(context);
        RPICmdPopEvent(context);
    }
}
