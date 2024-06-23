#include "yjw_pass_forward.h"
#include "Render/yjw_render_system.h"
#include "projectInfo.h"
#include "../yjw_scene.h"
#include "InternalShaderResource/yjw_internal_shader_resource.h"
namespace yjw
{
    void ForwardPass::buildPSO()
    {
    }

    void ForwardPass::registerTexture(
        RPITexture out_abedlo,
        RPITexture depth)
    {
        RPITexture texture[1] = { out_abedlo};
        renderPass = RPICreateRenderPass(texture, 1, depth);
        m_entitys = RenderSystem::get().scene->buildEntitys();
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
            m_entitys[i].m_material->SetDataVec2("metallic_roughness", glm::vec2(m_metallic, m_roughness));
            m_entitys[i].m_material->FlushDataToGpu();
        }
    }

    void ForwardPass::recordCommand(RPIContext commandBuffer)
    {
        std::vector<RPIResourceBinding> resource_bindings;
        resource_bindings.reserve(m_entitys.size());
        for (int i = 0; i < m_entitys.size(); i++)
        {
            resource_bindings.push_back(m_entitys[i].m_resource_binding);
        }
        RPICmdBeginRenderPass(commandBuffer, renderPass, resource_bindings.data(), m_entitys.size());
        for (int i = 0; i < m_entitys.size(); i++)
        {
            RPICmdSetPrimitiveBinding(commandBuffer, m_entitys[i].m_primitive_binding);
            RPICmdSetResourceBinding(commandBuffer, m_entitys[i].m_resource_binding);
            RPICmdSetPipeline(commandBuffer, m_entitys[i].m_material->GetPipeline());
            RPICmdDrawIndex(commandBuffer, 0, 1);
        }
        RPICmdEndPass(commandBuffer);
    }

    void ForwardPass::submit()
    {

    }
}