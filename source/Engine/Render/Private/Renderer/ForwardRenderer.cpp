#include "Engine/Render/Private/Renderer/ForwardRenderer.h"
#include "Engine/Render/Private/Pass/SkyBoxPass.h"
#include "Engine/Render/Private/Pass/ImGUIPass.h"
#include "projectInfo.h"

namespace yjw
{
    void ForwardRenderer::Initialize()
    {
        m_context = RPICreateContext();
        m_sky_box_pass = new SkyBoxPass();
        m_sky_box_pass->Initialize();
        m_sky_box_pass->LoadResource(RESOURCE_FILE(skybox/6));
        m_imgui_pass = new ImGuiPass();
        m_imgui_pass->Initialize();
    }

    void ForwardRenderer::Destroy()
    {
        m_sky_box_pass->Destroy();
        delete m_sky_box_pass;
        m_context->release();
    }

    void ForwardRenderer::SetOutput(RPITexture output_color, RPITexture output_depth)
    {
        m_output_color = output_color;
        m_output_depth = output_depth;
        RPITexture texture[1] = { m_output_color };
        if (m_render_pass)
        {
            m_render_pass->release();
        }
        m_render_pass = RPICreateRenderPass(texture, 1, m_output_depth);
    }

    void ForwardRenderer::SetRenderPass(RPIRenderPass render_pass)
    {
        if (m_render_pass)
        {
            m_render_pass->release();
        }
        m_render_pass = render_pass;
        m_render_pass->retain(nullptr);
        m_output_color = RPITexture::Null;
        m_output_depth = RPITexture::Null;
    }

    void ForwardRenderer::SetSceneProxy(RenderSceneProxy proxy)
    {
        m_scene_proxy = proxy;
    }

    void ForwardRenderer::SetUI(rhi::ImGuiUI* ui)
    {
        m_imgui_pass->LoadResource(ui);
    }

    void ForwardRenderer::BeginFrame()
    {
        if(m_output_color)RPICmdClearTexture(m_context, m_output_color);
        if(m_output_depth)RPICmdClearTexture(m_context, m_output_depth);
    }

    void ForwardRenderer::RenderFrame()
    {
        RPICmdBeginRenderPass(m_context, m_render_pass);
        SubmitOpacue();
        m_sky_box_pass->Submit(m_context);
        SubmitTransparent();
        m_imgui_pass->Submit(m_context);
        RPICmdEndPass(m_context);
    }

    void ForwardRenderer::EndFrame()
    {
        RPISubmit(m_context);
    }

    void ForwardRenderer::SubmitOpacue()
    {
        RPICmdPushEvent(m_context, "Opacue");
        m_scene_proxy.SubmitOpaque(this);
        RPICmdPopEvent(m_context);
    }

    void ForwardRenderer::Submit(DrawItem* item)
    {
        RPICmdSetPrimitiveBinding(m_context, item->m_primitive_binding, item->m_sub_primitive_id);
        RPICmdSetResourceBinding(m_context, item->m_resource_binding);
        RPICmdSetPipeline(m_context, item->m_pipeline);
        RPICmdDrawIndex(m_context, 0, 1);
    }

    void ForwardRenderer::SubmitTransparent()
    {

    }
}