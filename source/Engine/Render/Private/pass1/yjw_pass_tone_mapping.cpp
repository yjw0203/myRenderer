#include "yjw_pass_tone_mapping.h"
#include "Engine/Render/Public/yjw_render_system.h"
#include "projectInfo.h"
#include <memory>
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
namespace yjw
{
    void ToneMappingPass::buildPSO()
    {
        glm::vec2 vertex[6] = { glm::vec2(-1,-1),glm::vec2(1,-1),glm::vec2(-1,1),glm::vec2(1,-1),glm::vec2(1,1),glm::vec2(-1,1) };

        vertex_buffer = RPICreateGpuVertexBuffer(sizeof(vertex));
        RPIUpdateBuffer(vertex_buffer, vertex, 0, sizeof(vertex));

        vs = RPICreateShader(RPIShaderType::vertex, SHADER_FILE(tone_mapping.hlsl), "VSMain");
        ps = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(tone_mapping.hlsl), "PSMain");

        RPIRenderPipelineDescriptor pipelineDesc = RPIGetDefaultRenderPipeline();
        pipelineDesc.vs = vs;
        pipelineDesc.ps = ps;
        pipeline = RPICreateRenderPipeline(pipelineDesc);

        resourceBinding = RPICreateResourceBinding(pipeline);
        m_primitive_binding = RPICreatePrimitiveBinding(pipeline);
    }

    void ToneMappingPass::registerTexture(
        RPITexture in_color,
        RPITexture out_color)
    {
        renderPass = RPICreateRenderPass(&out_color, 1, RPITexture::Null);
        resourceBinding.SetTexture(RHIShaderType::fragment, RHIName("color_map"), in_color);
        m_primitive_binding.SetVertexBuffer(RHIName("POSITION"), vertex_buffer);
    }

    void ToneMappingPass::setupData()
    {
    }

    void ToneMappingPass::recordCommand(RPIContext commandBuffer)
    {
        RPICmdTransitionStateToRender(commandBuffer, &resourceBinding, 1);
        RPICmdBeginRenderPass(commandBuffer, renderPass);
        RPICmdSetResourceBinding(commandBuffer, resourceBinding);
        RPICmdSetPrimitiveBinding(commandBuffer, m_primitive_binding);
        RPICmdSetPipeline(commandBuffer, pipeline);
        RPICmdDraw(commandBuffer, 6, 1, 0, 0);
        RPICmdEndPass(commandBuffer);
    }

    void ToneMappingPass::submit()
    {

    }
}