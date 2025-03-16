#include "yjw_pass_draw_triangle.h"
#include "Engine/Render/Public/yjw_render_system.h"
#include "projectInfo.h"

namespace yjw
{
    void DrawTrianglePass::buildPSO()
    {
        vs = RPICreateShader(RPIShaderType::vertex, SHADER_FILE(draw_triangle.hlsl), "VSMain");
        ps = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(draw_triangle.hlsl), "PSMain");

        RPIRenderPipelineDescriptor pipelineDesc = RPIGetDefaultRenderPipeline();
        pipelineDesc.vs = vs;
        pipelineDesc.ps = ps;
        pipelineDesc.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_depth_read_and_write);
        pipeline = RPICreateRenderPipeline(pipelineDesc);
    }

    void DrawTrianglePass::registerTexture(RPITexture out_tex)
    {
        renderPass = RPICreateRenderPass(&out_tex, 1, RPITexture::Null);
    }

    void DrawTrianglePass::setupData()
    {
    }

    void DrawTrianglePass::recordCommand(RPIContext commandBuffer)
    {
        RPICmdBeginRenderPass(commandBuffer, renderPass);
        RPICmdSetPipeline(commandBuffer, pipeline);
        RPICmdDraw(commandBuffer, 6, 1, 0, 0);
        RPICmdEndPass(commandBuffer);
    }

    void DrawTrianglePass::submit()
    {

    }
}