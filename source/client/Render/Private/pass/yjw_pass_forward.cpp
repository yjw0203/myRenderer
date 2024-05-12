#include "yjw_pass_forward.h"
#include "Render/yjw_render_system.h"
#include "projectInfo.h"
#include "../yjw_resource.h"
#include "../yjw_scene.h"
namespace yjw
{
    void ForwardPass::buildPSO()
    {
        vs = RPICreateShader(RPIShaderType::vertex, SHADER_FILE(forward_pbr.hlsl), "VSMain");
        ps = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(forward_pbr.hlsl), "PSMain");

        RPIRenderPipelineDescriptor pipelineDesc = RPIGetDefaultRenderPipeline();
        pipelineDesc.vs = vs;
        pipelineDesc.ps = ps;
        pipelineDesc.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_depth_read_and_write);
        pipeline = RPICreateRenderPipeline(pipelineDesc);

        resource_bindings.resize(100);
        uniformsBuffers.resize(100);
        for (int i = 0; i < 100; i++)
        {
            resource_bindings[i] = RPICreateResourceBinding(pipeline);
            uniformsBuffers[i] = RPICreateUploadBuffer(16);
        }
    }

    void ForwardPass::registerTexture(
        RPITexture out_abedlo,
        RPITexture depth)
    {
        RPITexture texture[1] = { out_abedlo};
        renderPass = RPICreateRenderPass(texture, 1, depth);

        m_entitys = RenderSystem::get().scene->buildEntitys();
        for (int i = 0; i < m_entitys.size(); i++)
        {
            resource_bindings[i].SetBuffer(RHIShaderType::vertex, RHIName("camera"), g_resource_store.cameraUniform);
            resource_bindings[i].SetBuffer(RHIShaderType::fragment, RHIName("material"), uniformsBuffers[i]);
            resource_bindings[i].SetBuffer(RHIShaderType::fragment, RHIName("light"), g_resource_store.lightUniform);
            resource_bindings[i].SetTexture(RHIShaderType::fragment, RHIName("albedoTex"), m_entitys[i].material->textureShaderResource);
            resource_bindings[i].SetVertexBuffer(RHIName("POSITION"), m_entitys[i].mesh->vertex_buffers[0].buffer);
            resource_bindings[i].SetVertexBuffer(RHIName("NORMAL"), m_entitys[i].mesh->vertex_buffers[1].buffer);
            resource_bindings[i].SetVertexBuffer(RHIName("TEXCOORD0"), m_entitys[i].mesh->vertex_buffers[2].buffer);
            resource_bindings[i].SetIndexBuffer(m_entitys[i].mesh->index_buffer);
        }
    }

    void ForwardPass::setData(float metallic, float roughness)
    {
        m_metallic = metallic;
        m_roughness = roughness;
    }

    void ForwardPass::setupData()
    {
        for (int i = 0; i < m_entitys.size(); i++)
        {
            struct Material
            {
                alignas(16) glm::vec2 metallic_roughness;
            }material_data;
            material_data.metallic_roughness[0] = m_metallic;
            material_data.metallic_roughness[1] = m_roughness;

            RPIUpdateBuffer(uniformsBuffers[i], &material_data, 0, sizeof(Material));
        }
    }

    void ForwardPass::recordCommand(RPIContext commandBuffer)
    {
        RPICmdBeginRenderPass(commandBuffer, renderPass, resource_bindings.data(), m_entitys.size());
        for (int i = 0; i < m_entitys.size(); i++)
        {
            RPICmdSetResourceBinding(commandBuffer, resource_bindings[i]);
            RPICmdSetPipeline(commandBuffer, pipeline);
            RPICmdDrawIndex(commandBuffer, m_entitys[i].mesh->index_count, 1, m_entitys[i].mesh->first_index, 0, 0);
        }
        RPICmdEndPass(commandBuffer);
    }

    void ForwardPass::submit()
    {

    }
}