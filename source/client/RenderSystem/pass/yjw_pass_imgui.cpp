#include "yjw_pass_imgui.h"
#include "client/RenderSystem/yjw_render_system.h"
#include "generate/projectInfo.h"
#include "../yjw_resource.h"
#include "RHI/externs/imgui/3rd/imgui.h"

namespace yjw
{
    void DrawImGuiPass::buildPSO()
    {
        m_imgui_layer = dynamic_cast<RHIImguiLayer*>(RPIGetLayer(rhi_layer_imgui));
    }

    void DrawImGuiPass::registerTexture(RPITexture out_tex)
    {
        renderPass = RPICreateRenderPass(&out_tex, 1, RPITexture::Null);
    }

    void DrawImGuiPass::setupData()
    {
    }

    void DrawImGuiPass::recordCommand(RPIContext commandBuffer)
    {
        m_imgui_layer->NewFrame(commandBuffer, renderPass);
        ImGui::Text("Hello, world!");
        m_imgui_layer->Render(commandBuffer);
    }

    void DrawImGuiPass::submit()
    {

    }
}