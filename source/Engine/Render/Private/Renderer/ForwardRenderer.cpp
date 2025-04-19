#include "Engine/Render/Private/Renderer/ForwardRenderer.h"
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
#include "Engine/Render/Private/Pass/SkyBoxPass.h"
#include "Engine/Render/Private/Pass/ImGUIPass.h"
#include "Engine/Render/Private/Pass/PickPass.h"
#include "Engine/Render/Private/Pass/HighLightPass.h"
#include "projectInfo.h"

namespace yjw
{
    void ForwardRenderer::Initialize()
    {
        rpi::RPIRenderPipelineDescriptor pipelineDesc = rpi::RPIGetDefaultRenderPipeline();
        pipelineDesc.depth_stencil_state = rpi::RPIGetDepthStencilState(rpi::RPIDepthStencilStateType::default_depth_read_and_write);
        m_opacue_pipeline = RPICreateRenderPipeline(pipelineDesc);

        m_context = RPICreateContext();
        m_sky_box_pass = new SkyBoxPass();
        m_sky_box_pass->Initialize();
        m_sky_box_pass->LoadResource(RESOURCE_FILE(skybox/6));
        m_imgui_pass = new ImGuiPass();
        m_imgui_pass->Initialize();

        m_high_light_pass = new HighLightPass();
        m_high_light_pass->Initialize();
    }

    void ForwardRenderer::Destroy()
    {
        m_imgui_pass->Destroy();
        m_sky_box_pass->Destroy();
        m_high_light_pass->Destroy();
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

    void ForwardRenderer::SetView(RdView* view)
    {
        m_view = view;
        m_high_light_pass->AttachView(view);
    }

    void ForwardRenderer::Render()
    {
        if (m_view)
        {
            BeginFrame();
            RenderFrame();
            EndFrame();
        }
    }

    void ForwardRenderer::BeginFrame()
    {
        RPICmdClearBackBuffer(m_context, m_view->GetWindow());

        m_imgui_pass->LoadResource(m_view->GetUI());
        SetRenderPass(m_view->GetRenderPass());
        if(m_output_color)RPICmdClearTexture(m_context, m_output_color);
        if(m_output_depth)RPICmdClearTexture(m_context, m_output_depth);
    }

    void ForwardRenderer::RenderFrame()
    {
        m_high_light_pass->SubmitHighLightObject(m_context);
        RPICmdBeginRenderPass(m_context, m_render_pass);
        RPICmdSetResourceSet(m_context, RPIResourceSetType::common, g_internal_shader_parameters.GetCommonResourceSet());
        SubmitOpacue();
        m_sky_box_pass->Submit(m_context);
        SubmitTransparent();
        m_high_light_pass->HighLightPostProcess(m_context);
        //m_imgui_pass->Submit(m_context);
        RPICmdEndPass(m_context);
    }

    void ForwardRenderer::EndFrame()
    {
        RPISubmit(m_context);
        rpi::RPIPresent(m_context, m_view->GetWindow());
    }

    void ForwardRenderer::SubmitOpacue()
    {
        RPICmdPushEvent(m_context, "Opacue");
        RenderSceneProxy(m_view->GetScene()).SubmitOpaque(this, &ForwardRenderer::Submit);
        RPICmdPopEvent(m_context);
    }

    void ForwardRenderer::Submit(DrawItem* item)
    {
        RPICmdSetPrimitiveBinding(m_context, item->m_primitive->GetPrimitiveBinding(), item->m_sub_primitive_id);
        RPICmdSetResourceSet(m_context, RPIResourceSetType::vs, item->m_primitive->GetVSResourceSet());
        RPICmdSetResourceSet(m_context, RPIResourceSetType::ps, item->m_material->GetResourceSet());
        RPICmdSetRenderPipeline(m_context, m_opacue_pipeline, item->m_primitive->GetVertexShader(), item->m_material->GetPixelShader());
        RPICmdDrawIndex(m_context, 0, 1);
    }

    void ForwardRenderer::SubmitTransparent()
    {

    }
}