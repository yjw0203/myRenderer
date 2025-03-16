#pragma once
#include "Engine/Render/Private/RenderSceneProxy.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/Render/Private/Renderer/IRenderer.h"

namespace yjw
{
    using namespace rpi;
    class SkyBoxPass;
    class ForwardRenderer : public IRenderer
    {
    public:
        ForwardRenderer() {}
        void Initialize();
        void Destroy();
        void SetOutput(RPITexture output_color, RPITexture output_depth);
        void SetRenderPass(RPIRenderPass render_pass);
        void SetSceneProxy(RenderSceneProxy proxy);

        void BeginFrame();
        void RenderFrame();
        void EndFrame();

        virtual void Submit(RenderEntity* entity) override;
    private:
        void SubmitOpacue();
        void SubmitTransparent();

    private:
        RenderSceneProxy m_scene_proxy{};
        RPIContext m_context{};

        RPITexture m_output_color{};
        RPITexture m_output_depth{};
        RPIRenderPass m_render_pass{};

        SkyBoxPass* m_sky_box_pass{};
    };
}