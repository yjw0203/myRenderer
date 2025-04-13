#pragma once
#include "Engine/Render/Private/RenderSceneProxy.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/Render/Private/Renderer/IRenderer.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"
#include "Engine/Render/Private/View.h"

namespace yjw
{
    using namespace rpi;
    class SkyBoxPass;
    class ImGuiPass;
    class HighLightPass;
    class ForwardRenderer : public IRenderer
    {
    public:
        ForwardRenderer() {}
        void Initialize();
        void Destroy();
        void SetOutput(RPITexture output_color, RPITexture output_depth);
        void SetRenderPass(RPIRenderPass render_pass);
        void SetView(RView* view);

        void Render();
    private:

        void BeginFrame();
        void RenderFrame();
        void EndFrame();

        void Submit(DrawItem* item);
        void SubmitOpacue();
        void SubmitTransparent();

    private:
        RView* m_view{};
        RPIContext m_context{};

        RPITexture m_output_color{};
        RPITexture m_output_depth{};
        RPIRenderPass m_render_pass{};

        RPIRenderPipeline m_opacue_pipeline{};

        SkyBoxPass* m_sky_box_pass{};
        ImGuiPass* m_imgui_pass{};
        HighLightPass* m_high_light_pass{};
        
    };
}