#include "yjw_pass_debug.h"
#include "Engine/Render/Public/yjw_render_system.h"
#include "projectInfo.h"
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"

namespace yjw
{
    void DebugPass::buildPSO()
    {
        vs = RPICreateShader(RPIShaderType::vertex, SHADER_FILE(debug.hlsl), "VSMain");
        ps = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(debug.hlsl), "PSMain");

        RPIRenderPipelineDescriptor pipelineDesc = RPIGetDefaultRenderPipeline();
        pipelineDesc.vs = vs;
        pipelineDesc.ps = ps;
        pipelineDesc.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_depth_read_and_write);
        pipelineDesc.rasterization_state.cull_mode = RHICullMode::cull_mode_none;
        pipelineDesc.primitiveTopology = RHIPrimitiveTopology::primitive_topology_line_list;
        pipeline = RPICreateRenderPipeline(pipelineDesc);

        resourceBinding = RPICreateResourceBinding(pipeline);
        m_primitive_binding = RPICreatePrimitiveBinding(pipeline);

        vertex_buffer = RPICreateUploadBuffer(m_max_line_count * sizeof(glm::vec3) * 2);
    }

    void DebugPass::registerTexture(RPITexture out_tex)
    {
        renderPass = RPICreateRenderPass(&out_tex, 1, RPITexture::Null);
        resourceBinding.SetBuffer(RHIShaderType::fragment, RHIName("camera"), g_internal_shader_parameters.GetGpuBufferByShaderParameterName("camera"));
        m_primitive_binding.SetVertexBuffer(RHIName("POSITION"), vertex_buffer);
    }

    void DebugPass::Clear()
    {
        m_lines.clear();
    }

    void DebugPass::AddLine(glm::vec3 point0, glm::vec3 point1)
    {
        m_lines.push_back(point0);
        m_lines.push_back(point1);
    }

    void DebugPass::setupData()
    {
        if (m_lines.size())
        {
            RPIUpdateBuffer(vertex_buffer, m_lines.data(), 0, m_lines.size() * sizeof(glm::vec3));
            m_line_count = m_lines.size() / 2;
        }
    }

    void DebugPass::recordCommand(RPIContext commandBuffer)
    {
        RPICmdPushEvent(commandBuffer, "Debug");
        RPICmdBeginRenderPass(commandBuffer, renderPass, &resourceBinding, 1);
        RPICmdSetResourceBinding(commandBuffer, resourceBinding);
        RPICmdSetPrimitiveBinding(commandBuffer, m_primitive_binding);
        RPICmdSetPipeline(commandBuffer, pipeline);
        RPICmdDraw(commandBuffer, m_line_count * 2, 1, 0, 0);
        RPICmdEndPass(commandBuffer);
        RPICmdPopEvent(commandBuffer);
    }

    void DebugPass::submit()
    {

    }
}