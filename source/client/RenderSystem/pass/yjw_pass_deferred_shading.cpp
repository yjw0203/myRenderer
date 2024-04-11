#include "yjw_pass_deferred_shading.h"
#include "client/RenderSystem/yjw_render_system.h"
#include "generate/projectInfo.h"
#include <memory>
#include "../yjw_resource.h"
namespace yjw
{
    void DeferredShadingPass::buildPSO()
    {
        glm::vec2 vertex[6] = { glm::vec2(-1,-1),glm::vec2(1,-1),glm::vec2(-1,1),glm::vec2(1,-1),glm::vec2(1,1),glm::vec2(-1,1) };

        vertex_buffer = RPICreateGpuVertexBuffer(sizeof(vertex));
        RPIUpdateBuffer(vertex_buffer, vertex, 0, sizeof(vertex));

        vs = RPICreateShader(SHADER_FILE(deferred_shading_vert.spv));
        ps = RPICreateShader(SHADER_FILE(deferred_shading_frag.spv));

        RPIRenderPipelineDescriptor pipelineDesc = RPIGetDefaultRenderPipeline();
        pipelineDesc.vs = vs;
        pipelineDesc.vs_entry = "main";
        pipelineDesc.ps = ps;
        pipelineDesc.ps_entry = "main";
        pipeline = RPICreateRenderPipeline(pipelineDesc);

        resourceBinding = RPICreateResourceBinding(pipeline);
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
        this->in_abeldo = RPICreateTextureView(in_abeldo, RPIFormat::R8G8B8A8_snorm);
        this->in_normal = RPICreateTextureView(in_normal, RPIFormat::R8G8B8A8_snorm);
        this->in_diffuse = RPICreateTextureView(in_diffuse, RPIFormat::R32G32B32A32_sfloat);
        this->in_specular = RPICreateTextureView(in_specular, RPIFormat::R32G32B32A32_sfloat);
        this->in_ambient = RPICreateTextureView(in_ambient, RPIFormat::R32G32B32A32_sfloat);
        this->in_depth = RPICreateTextureView(in_depth,  RPIFormat::D24_unorm_S8_uint);
        this->out_color = RPICreateTextureView(out_color, RPIFormat::R8G8B8A8_unorm);
        renderPass = RPICreateRenderPass(&this->out_color, 1, RPINull);

        resourceBinding->SetBufferView(RHIShaderType::fragment, RHIName("camera"), g_resource_store.cameraUniformDescriptor);
        resourceBinding->SetBufferView(RHIShaderType::fragment, RHIName("light"), g_resource_store.lightUniformDescriptor);
        resourceBinding->SetBufferView(RHIShaderType::fragment, RHIName("option"), g_resource_store.optionUniformDescriptor);
        resourceBinding->SetTextureView(RHIShaderType::fragment, RHIName("albedo_map"), this->in_abeldo);
        resourceBinding->SetTextureView(RHIShaderType::fragment, RHIName("normal_map"), this->in_normal);
        resourceBinding->SetTextureView(RHIShaderType::fragment, RHIName("diffuse_map"), this->in_diffuse);
        resourceBinding->SetTextureView(RHIShaderType::fragment, RHIName("specular_map"), this->in_specular);
        resourceBinding->SetTextureView(RHIShaderType::fragment, RHIName("ambient_map"), this->in_ambient);
        resourceBinding->SetTextureView(RHIShaderType::fragment, RHIName("depth_map"), this->in_depth);
        resourceBinding->SetVertexBuffer(RHIName("in_pos"), vertex_buffer);
    }

    void DeferredShadingPass::setupData()
    {
    }

    void DeferredShadingPass::recordCommand(RPIContext commandBuffer)
    {
        RPICmdBeginRenderPass(commandBuffer, renderPass, &resourceBinding, 1);
        RPICmdSetResourceBinding(commandBuffer, resourceBinding);
        RPICmdSetPipeline(commandBuffer, pipeline);
        RPICmdDraw(commandBuffer, 6, 1, 0, 0);
        RPICmdEndPass(commandBuffer);
    }

    void DeferredShadingPass::submit()
    {

    }
}