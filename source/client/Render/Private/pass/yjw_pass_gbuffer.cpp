#include "yjw_pass_gbuffer.h"
#include "Render/yjw_render_system.h"
#include "projectInfo.h"
#include "../yjw_scene.h"
#include "InternalShaderResource/yjw_internal_shader_resource.h"
namespace yjw
{
    void GBufferPass::buildPSO()
    {
        vs = RPICreateShader(RPIShaderType::vertex, SHADER_FILE(gbuffer.hlsl), "VSMain");
        ps = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(gbuffer.hlsl), "PSMain");

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
            uniformsBuffers[i] = RPICreateUploadBuffer(48);
        }
    }

    void GBufferPass::registerTexture(
        RPITexture out_abedlo,
        RPITexture out_normal,
        RPITexture out_diffuse,
        RPITexture out_specular,
        RPITexture out_ambient,
        RPITexture depth)
    {
        /*
        RPITexture texture[5] = { out_abedlo,out_normal ,out_diffuse ,out_specular ,out_ambient };
        renderPass = RPICreateRenderPass(texture, 5, depth);

        m_entitys = RenderSystem::get().scene->buildEntitys();
        for (int i = 0; i < m_entitys.size(); i++)
        {
            resource_bindings[i].SetBuffer(RHIShaderType::vertex, RHIName("camera"), g_internal_shader_parameters.GetGpuBufferByShaderParameterName("camera"));
            resource_bindings[i].SetBuffer(RHIShaderType::fragment, RHIName("material"), uniformsBuffers[i]);
            resource_bindings[i].SetTexture(RHIShaderType::fragment, RHIName("albedoTex"), m_entitys[i].material->textureShaderResource);
            resource_bindings[i].SetVertexBuffer(RHIName("POSITION"), m_entitys[i].mesh->vertex_buffers[0].buffer);
            resource_bindings[i].SetVertexBuffer(RHIName("NORMAL"), m_entitys[i].mesh->vertex_buffers[1].buffer);
            resource_bindings[i].SetVertexBuffer(RHIName("TEXCOORD0"), m_entitys[i].mesh->vertex_buffers[2].buffer);
            resource_bindings[i].SetIndexBuffer(m_entitys[i].mesh->index_buffer);
        }*/
    }

    void GBufferPass::setupData()
    {
        /*
        for (int i = 0; i < m_entitys.size(); i++)
        {
            struct Material
            {
                alignas(16) glm::vec4 diffuse;
                alignas(16) glm::vec4 specular;
                alignas(16) glm::vec3 ambient;
            }material_data;
            material_data.diffuse = m_entitys[i].material->diffuse;
            material_data.specular = glm::vec4(m_entitys[i].material->specular, m_entitys[i].material->specularPower);
            material_data.ambient = m_entitys[i].material->ambient;

            RPIUpdateBuffer(uniformsBuffers[i], &material_data, 0, sizeof(Material));
        }
        */
    }

    void GBufferPass::recordCommand(RPIContext commandBuffer)
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

    void GBufferPass::submit()
    {

    }
}