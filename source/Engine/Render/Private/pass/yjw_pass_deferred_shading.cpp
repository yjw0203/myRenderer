#include "yjw_pass_deferred_shading.h"
#include "Engine/Render/Public/yjw_render_system.h"
#include "projectInfo.h"
#include <memory>
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
namespace yjw
{
    void DeferredShadingPass::buildPSO()
    {
        glm::vec2 vertex[6] = { glm::vec2(-1,-1),glm::vec2(1,-1),glm::vec2(-1,1),glm::vec2(1,-1),glm::vec2(1,1),glm::vec2(-1,1) };

        vertex_buffer = RPICreateGpuVertexBuffer(sizeof(vertex));
        RPIUpdateBuffer(vertex_buffer, vertex, 0, sizeof(vertex));

        vs = RPICreateShader(RPIShaderType::vertex, SHADER_FILE(deferred_shading.hlsl), "VSMain");
        ps = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(deferred_shading.hlsl), "PSMain");

        RPIRenderPipelineDescriptor pipelineDesc = RPIGetDefaultRenderPipeline();
        pipelineDesc.vs = vs;
        pipelineDesc.ps = ps;
        pipeline = RPICreateRenderPipeline(pipelineDesc);

        resourceBinding = RPICreateResourceBinding(pipeline);
        m_primitive_binding = RPICreatePrimitiveBinding(pipeline);
    }

    void DeferredShadingPass::registerTexture(
        RPITexture in_abeldo,
        RPITexture in_normal,
        RPITexture in_diffuse,
        RPITexture in_specular,
        RPITexture in_ambient,
        RPITexture in_depth,
        RPITexture out_color)
    {
        renderPass = RPICreateRenderPass(&out_color, 1, RPITexture::Null);

        resourceBinding.SetBuffer(RHIShaderType::fragment, RHIName("camera"), g_internal_shader_parameters.GetGpuBufferByShaderParameterName("camera"));
        resourceBinding.SetBuffer(RHIShaderType::fragment, RHIName("light"), g_internal_shader_parameters.GetGpuBufferByShaderParameterName("light"));
        resourceBinding.SetBuffer(RHIShaderType::fragment, RHIName("option"), g_internal_shader_parameters.GetGpuBufferByShaderParameterName("option"));
        resourceBinding.SetTexture(RHIShaderType::fragment, RHIName("albedo_map"), in_abeldo);
        resourceBinding.SetTexture(RHIShaderType::fragment, RHIName("normal_map"), in_normal);
        resourceBinding.SetTexture(RHIShaderType::fragment, RHIName("diffuse_map"), in_diffuse);
        resourceBinding.SetTexture(RHIShaderType::fragment, RHIName("specular_map"), in_specular);
        resourceBinding.SetTexture(RHIShaderType::fragment, RHIName("ambient_map"), in_ambient);
        resourceBinding.SetTexture(RHIShaderType::fragment, RHIName("depth_map"), in_depth);
        m_primitive_binding.SetVertexBuffer(RHIName("POSITION"), vertex_buffer);
    }

    void DeferredShadingPass::setupData()
    {
    }

    void DeferredShadingPass::recordCommand(RPIContext commandBuffer)
    {
        RPICmdTransitionStateToRender(commandBuffer, &resourceBinding, 1);
        RPICmdBeginRenderPass(commandBuffer, renderPass);
        RPICmdSetResourceBinding(commandBuffer, resourceBinding);
        RPICmdSetPrimitiveBinding(commandBuffer, m_primitive_binding);
        RPICmdSetPipeline(commandBuffer, pipeline);
        RPICmdDraw(commandBuffer, 6, 1, 0, 0);
        RPICmdEndPass(commandBuffer);
    }

    void DeferredShadingPass::submit()
    {

    }
}