#include "Engine/Render/Private/Pass/PickPass.h"
#include "Engine/Render/Private/RenderSceneProxy.h"
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
#include "projectInfo.h"

namespace yjw
{
    void PickPass::Initialize()
    {
        m_ps = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(Pick.hlsl), "PSMain");
        RPIRenderPipelineDescriptor pipelineDesc = RPIGetDefaultRenderPipeline();
        pipelineDesc.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_depth_read_and_write);
        m_pipeline = RPICreateRenderPipeline(pipelineDesc);
        m_pick_output_texture = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R32G32B32A32_sint);
        m_pick_staging_texture = RPICreateReadbackTexture2D(1200, 1200, RPIFormat::R32G32B32A32_sint);
        m_pick_depth_texture = RPICreateDepthStencilTexture2D(1200, 1200, RPIFormat::D24_unorm_S8_uint);

        m_render_pass = RPICreateRenderPass(&m_pick_output_texture, 1, m_pick_depth_texture);

        m_context = RPICreateContext();
    }

    void PickPass::Destroy()
    {
        m_pick_output_texture.Release();
        m_pick_staging_texture.Release();
        m_pick_depth_texture.Release();
    }

    void PickPass::AttachView(RdView* view)
    {
        m_view = view;
    }

    void PickPass::Submit()
    {
        RPICmdPushEvent(m_context, "PickPass");
        RPICmdClearTexture(m_context, m_pick_output_texture);
        RPICmdClearTexture(m_context, m_pick_depth_texture);
        RPICmdBeginRenderPass(m_context, m_render_pass);
        RPICmdSetResourceSet(m_context, RPIResourceSetType::common, g_internal_shader_parameters.GetCommonResourceSet());
        RPICmdSetResourceSet(m_context, RPIResourceSetType::entity, m_view->GetScene()->GetEntityResourceSet());
        RenderSceneProxy(m_view->GetScene()).SubmitOpaque(this, &PickPass::SubmitOpacue);
        RPICmdEndPass(m_context);
        RPICmdCopyTexture(m_context, m_pick_output_texture, m_pick_staging_texture);
        RPICmdPopEvent(m_context);
        RPIFence fence = RPISubmit(m_context);
        RPIWaitForFence(fence);
        int data[4] = {};
        m_pick_staging_texture.BeginReadbackMode(0, 0);
        for (std::pair<std::string, RdHitRequestStruct>& req : m_pending_request)
        {
            int result[4] = {};
            m_pick_staging_texture.ReadbackLoad(req.second.m_posx, req.second.m_posy, result);
            req.second.m_result = result[3];
            req.second.m_completed = true;
            m_processed_request[req.first].push_back(req.second);
        }
        m_pending_request.clear();
        m_pick_staging_texture.EndReadbackMode();
    }

    void PickPass::AddPendingHitRequest(const char* group_name, const RdHitRequestStruct& request)
    {
        m_pending_request.push_back(std::make_pair(std::string(group_name), request));
    }

    void PickPass::GetProcessedHitRequest(const char* group_name, std::vector<RdHitRequestStruct>& proccessed_request)
    {
        if (m_processed_request.find(group_name) != m_processed_request.end())
        {
            proccessed_request.swap(m_processed_request[group_name]);
            m_processed_request.erase(group_name);
        }
    }

    void PickPass::SubmitOpacue(DrawItem* item)
    {
        RPICmdPushConstants(m_context, item->m_entity->GetPushContants(), 0, sizeof(int) * 4);
        RPICmdSetPrimitiveBinding(m_context, item->m_primitive->GetPrimitiveBinding(), item->m_sub_primitive_id);
        RPICmdSetResourceSet(m_context, RPIResourceSetType::vs, item->m_primitive->GetVSResourceSet());
        RPICmdSetRenderPipeline(m_context, m_pipeline, item->m_primitive->GetVertexShader(), m_ps);
        RPICmdDrawIndex(m_context, 0, 1);
    }
}