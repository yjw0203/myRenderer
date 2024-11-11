#include "yjw_pass_forward.h"
#include "Render/yjw_render_system.h"
#include "projectInfo.h"
#include "../yjw_scene.h"
#include "InternalShaderResource/yjw_internal_shader_resource.h"
namespace yjw
{
    void ForwardPass::buildPSO()
    {
        m_bone_matrices = RPICreateFormatBuffer(sizeof(Matrix4x4) * m_max_bone_count, RPIFormat::R32G32B32A32_sfloat);
    }

    void ForwardPass::registerTexture(
        RPITexture out_abedlo,
        RPITexture depth)
    {
        RPITexture texture[1] = { out_abedlo};
        renderPass = RPICreateRenderPass(texture, 1, depth);
    }

    void ForwardPass::attachScene(Scene* scene)
    {
        m_scene = scene;
    }

    void ForwardPass::setData()
    {
        m_entitys = m_scene->buildEntitys();
    }

    void ForwardPass::setupData()
    {
        for (int i = 0; i < m_entitys.size(); i++)
        {
            m_entitys[i].m_material->SetDataVec2("metallic_roughness", glm::vec2(0.5, 0.5));
            m_entitys[i].m_material->FlushDataToGpu();
        }
    }

    void ForwardPass::setBoneMatrices(const std::vector<Matrix4x4>& matrices)
    {
        RPIUpdateBuffer(m_bone_matrices, (void*)matrices.data(), 0, matrices.size() * sizeof(Matrix4x4));
    }

    void ForwardPass::recordCommand(RPIContext commandBuffer)
    {
        RPICmdPushEvent(commandBuffer, "ForwardPass");
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
        RPICmdPopEvent(commandBuffer);
    }

    void ForwardPass::submit()
    {

    }
}