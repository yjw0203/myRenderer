#include "Engine/Render/Private/Scene.h"
#include "Engine/Engine/Public/Framework/Components/PrimitiveComponent.h"
#include "Engine/Render/Private/Mesh.h"
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
#include <queue>

namespace yjw
{
    RenderEntity::RenderEntity()
    {
        m_resource_binding = rpi::RPICreateResourceBinding();
    }

    RenderEntity::~RenderEntity()
    {
        m_resource_binding.Release();
        if (m_pipeline)
        {
            m_pipeline->release();
        }
    }
    bool RenderEntity::ReadyRender()
    {
        return m_builded;
    }

    void RenderEntity::Build()
    {
        if (!m_builded)
        {
            if(m_primitive) m_primitive->BuildGpuPrimitive();
            if (m_primitive && m_material && m_primitive->GetVertexShader() && m_material->GetPixelShader())
            {
                m_primitive_binding = m_primitive->GetPrimitiveBinding();
                rpi::RPIRenderPipelineDescriptor pipelineDesc = rpi::RPIGetDefaultRenderPipeline();
                pipelineDesc.vs = m_primitive->GetVertexShader();
                pipelineDesc.ps = m_material->GetPixelShader();
                pipelineDesc.depth_stencil_state = rpi::RPIGetDepthStencilState(rpi::RPIDepthStencilStateType::default_depth_read_and_write);
                m_pipeline = rpi::RPICreateRenderPipeline(pipelineDesc);

                m_resource_binding.SetResourceSet(rpi::RPIResourceSetType::common, g_internal_shader_parameters.GetCommonResourceSet());
                m_resource_binding.SetResourceSet(rpi::RPIResourceSetType::vs, m_primitive->GetVSResourceSet());
                m_resource_binding.SetResourceSet(rpi::RPIResourceSetType::ps, m_material->GetResourceSet());
                m_builded = true;
            }
        }
    }

    Scene::Scene()
    {
        for (RenderEntity* entity : m_meshes)
        {
            delete entity;
        }
        m_meshes.clear();
    }

    void Scene::AddPrimitiveComponent(PrimitiveComponent* mesh)
    {
        RenderEntity* entity = new RenderEntity();
        entity->m_primitive_ptr = mesh;
        entity->m_material = mesh->GetMaterialInstance();
        entity->m_primitive = mesh->GetPrimitive();
        m_meshes.push_back(entity);
    }

    void Scene::RemovePrimitiveComponent(PrimitiveComponent* mesh)
    {
        for (int i = 0; i < m_meshes.size(); i++)
        {
            if (m_meshes[i]->m_primitive_ptr == mesh)
            {
                delete m_meshes[i];
                m_meshes[i] = m_meshes.back();
                m_meshes.resize(m_meshes.size() - 1);
            }
        }
    }

    std::vector<RenderEntity*>& Scene::GetMeshes()
    {
        return m_meshes;
    }

    void Scene::Update(float deltaTime)
    {

    }

}