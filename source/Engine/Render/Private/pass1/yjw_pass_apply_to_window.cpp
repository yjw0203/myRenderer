#include "yjw_pass_apply_to_window.h"
#include "Engine/Render/Public/yjw_render_system.h"
#include "projectInfo.h"
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"

namespace yjw
{
    ApplyToWindowPass::ApplyToWindowPass(RPIWindow window)
    {
        m_window = window;
    }

    void ApplyToWindowPass::buildPSO()
    {
        glm::vec2 vertex[6] = { glm::vec2(-1,-1),glm::vec2(1,-1),glm::vec2(-1,1),glm::vec2(1,-1),glm::vec2(1,1),glm::vec2(-1,1) };

        vertex_buffer = RPICreateGpuVertexBuffer(sizeof(vertex));
        RPIUpdateBuffer(vertex_buffer, vertex, 0, sizeof(vertex));

        vs = RPICreateShader(RPIShaderType::vertex, SHADER_FILE(apply_to_window.hlsl), "VSMain");
        ps = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(apply_to_window.hlsl), "PSMain");

        RPIRenderPipelineDescriptor pipelineDesc = RPIGetDefaultRenderPipeline();
        pipelineDesc.vs = vs;
        pipelineDesc.ps = ps;
        pipeline = RPICreateRenderPipeline(pipelineDesc);
        resourceBinding = RPICreateResourceBinding(pipeline);
        m_primitive_binding = RPICreatePrimitiveBinding(pipeline);
    }

    void ApplyToWindowPass::registerTexture(RPITexture apply_tex)
    {
        resourceBinding.SetBuffer(RHIShaderType::fragment, RHIName("option"), g_internal_shader_parameters.GetGpuBufferByShaderParameterName("option"));
        resourceBinding.SetTexture(RHIShaderType::fragment, RHIName("applyTex"), apply_tex);
        m_primitive_binding.SetVertexBuffer(RHIName("POSITION"), vertex_buffer);
    }

    void ApplyToWindowPass::setupData()
    {
        renderPass = m_window.swapchain->GetCurrentRenderPass();
    }

    void ApplyToWindowPass::recordCommand(RPIContext commandBuffer)
    {
        RPICmdTransitionStateToRender(commandBuffer, &resourceBinding, 1);
        RPICmdBeginRenderPass(commandBuffer, renderPass);
        RPICmdSetPipeline(commandBuffer, pipeline);
        RPICmdSetResourceBinding(commandBuffer, resourceBinding);
        RPICmdSetPrimitiveBinding(commandBuffer, m_primitive_binding);
        RPICmdDraw(commandBuffer, 6, 1, 0, 0);
        RPICmdEndPass(commandBuffer);
    }

    void ApplyToWindowPass::submit()
    {

    }
}