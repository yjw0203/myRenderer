#include "yjw_pass_forward.h"
#include "Render/yjw_render_system.h"
#include "projectInfo.h"
#include "../yjw_scene.h"
#include "InternalShaderResource/yjw_internal_shader_resource.h"
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
            m_entitys[i].material->GetResourceBinding().SetVertexBuffer(RHIName("POSITION"), m_entitys[i].mesh->vertex_buffers[0].buffer);
            m_entitys[i].material->GetResourceBinding().SetVertexBuffer(RHIName("NORMAL"), m_entitys[i].mesh->vertex_buffers[1].buffer);
            m_entitys[i].material->GetResourceBinding().SetVertexBuffer(RHIName("TEXCOORD0"), m_entitys[i].mesh->vertex_buffers[2].buffer);
            m_entitys[i].material->GetResourceBinding().SetIndexBuffer(m_entitys[i].mesh->index_buffer);
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
            m_entitys[i].material->SetDataVec2("metallic_roughness", glm::vec2(m_metallic, m_roughness));
        }
    }

    void ForwardPass::recordCommand(RPIContext commandBuffer)
    {
        std::vector<RPIResourceBinding> resource_bindings;
        resource_bindings.reserve(m_entitys.size());
        for (int i = 0; i < m_entitys.size(); i++)
        {
            resource_bindings.push_back(m_entitys[i].material->GetResourceBinding());
        }
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