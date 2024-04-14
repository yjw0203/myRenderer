#include "yjw_pass_apply_to_window.h"
#include "client/RenderSystem/yjw_render_system.h"
#include "generate/projectInfo.h"
#include "../yjw_resource.h"

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
    }

    void ApplyToWindowPass::registerTexture(RPITexture apply_tex)
    {
        resourceBinding.SetTexture(RHIShaderType::fragment, RHIName("applyTex"), apply_tex);
        resourceBinding.SetVertexBuffer(RHIName("POSITION"), vertex_buffer);
    }

    void ApplyToWindowPass::setupData()
    {
        renderPass = m_window.swapchain->GetCurrentRenderPass();
    }

    void ApplyToWindowPass::recordCommand(RPIContext commandBuffer)
    {
        RPICmdBeginRenderPass(commandBuffer, renderPass, &resourceBinding, 1);
        RPICmdSetPipeline(commandBuffer, pipeline);
        RPICmdSetResourceBinding(commandBuffer, resourceBinding);
        RPICmdDraw(commandBuffer, 6, 1, 0, 0);
        RPICmdEndPass(commandBuffer);
    }

    void ApplyToWindowPass::submit()
    {

    }
}