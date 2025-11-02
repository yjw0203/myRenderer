#include "Engine/Render/Private/Pass/HighLightPass.h"
#include "Engine/Render/Private/RenderSceneProxy.h"
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
#include "projectInfo.h"

void HighLightPass::Initialize()
{
    m_highlight_mask_ps = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(HighLight.hlsl), "Mask");
    RPIRenderPipelineDescriptor pipelineDesc = RPIGetDefaultRenderPipeline();
    pipelineDesc.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_depth_read_and_write);
    m_highlight_mask_pipeline = RPICreateRenderPipeline(pipelineDesc);
    m_highlight_mask_normal = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R32G32B32A32_sfloat);
    m_highlight_mask_depth = RPICreateDepthStencilTexture2D(1200, 1200, RPIFormat::D24_unorm_S8_uint);

    m_highlight_mask_render_pass = RPICreateRenderPass(&m_highlight_mask_normal, 1, m_highlight_mask_depth);

    m_highlight_post_process_vs = RPICreateShader(RPIShaderType::vertex, SHADER_FILE(HighLight.hlsl), "PostProcessVS");
    m_highlight_post_process_ps = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(HighLight.hlsl), "PostProcessPS");
    RPIRenderPipelineDescriptor pipelineDesc1 = RPIGetDefaultRenderPipeline();
    pipelineDesc1.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_no_depth_no_stencil);
    pipelineDesc1.color_blend_state[0] = RPIGetColorBlendState(RPIColorBlendStateType::default_alpha_blend);
    m_highlight_post_process_pipeline = RPICreateRenderPipeline(pipelineDesc1);

    m_post_process_Primitive_binding = RPICreatePrimitiveBinding(m_highlight_post_process_vs);
    glm::vec2 vertex[6] = { glm::vec2(-1,-1),glm::vec2(1,-1),glm::vec2(-1,1),glm::vec2(1,-1),glm::vec2(1,1),glm::vec2(-1,1) };
    m_post_process_vb = RPICreateGpuVertexBuffer(sizeof(vertex));
    RPIUpdateBuffer(m_post_process_vb, vertex, 0, sizeof(vertex));
    m_post_process_Primitive_binding.SetVertexBuffer(RHIName("POSITION"), m_post_process_vb);
    m_high_light_post_process_ps_resource_set = RPICreateResourceSet(RPIResourceSetType::ps, m_highlight_post_process_ps->GetShaderReflect());
    m_high_light_post_process_ps_resource_set.SetTexture(RHIName("depthTex"), m_highlight_mask_depth);
    m_high_light_post_process_ps_resource_set.SetTexture(RHIName("normalTex"), m_highlight_mask_normal);
}

void HighLightPass::Destroy()
{
    m_highlight_mask_render_pass->release();
    m_highlight_mask_normal.Release();
    m_highlight_mask_depth.Release();
}

void HighLightPass::AttachView(RdView* view)
{
    m_view = view;
}

void HighLightPass::SubmitHighLightObject(RPIContext context)
{
    m_current_context = context;
    RPICmdPushEvent(context, "HighLightPass");
    RPICmdClearTexture(context, m_highlight_mask_normal);
    RPICmdClearTexture(context, m_highlight_mask_depth);
    RPICmdBeginRenderPass(context, m_highlight_mask_render_pass);
    RPICmdSetResourceSet(context, RPIResourceSetType::common, g_internal_shader_parameters.GetCommonResourceSet());
    RPICmdSetResourceSet(context, RPIResourceSetType::entity, m_view->GetScene()->GetEntityResourceSet());
    RenderSceneProxy(m_view->GetScene()).SubmitOpaque(this, &HighLightPass::SubmitOpacue);
    RPICmdEndPass(context);
    RPICmdPopEvent(context);
}

void HighLightPass::SubmitOpacue(DrawItem* item)
{
    if (item->m_entity->GetRenderMask(RdRenderMaskBits::highlight))
    {
        RPICmdPushConstants(m_current_context, item->m_entity->GetPushContants(), 0, sizeof(int) * 4);
        RPICmdSetPrimitiveBinding(m_current_context, item->m_primitive->GetPrimitiveBinding(), item->m_sub_primitive_id);
        RPICmdSetResourceSet(m_current_context, RPIResourceSetType::vs, item->m_primitive->GetVSResourceSet());
        RPICmdSetRenderPipeline(m_current_context, m_highlight_mask_pipeline, item->m_primitive->GetPrimitiveBinding(), m_highlight_mask_ps);
        RPICmdDrawIndex(m_current_context, 0, 1);
    }
}

void HighLightPass::HighLightPostProcess(RPIContext context)
{
    RPICmdPushEvent(context, "HighLightPostProcess");
    RPICmdSetResourceSet(context, RPIResourceSetType::ps, m_high_light_post_process_ps_resource_set);
    RPICmdSetPrimitiveBinding(context, m_post_process_Primitive_binding, 0);
    RPICmdSetRenderPipeline(context, m_highlight_post_process_pipeline, m_post_process_Primitive_binding, m_highlight_post_process_ps);
    RPICmdDraw(context, 6, 1, 0, 0);
    RPICmdPopEvent(context);
}
