#include "Engine/Render/Private/Scene.h"
#include "Engine/Engine/Public/Framework/Components/PrimitiveComponent.h"
#include "Engine/Render/Private/Mesh.h"
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
#include "Engine/Render/Private/MaterialManager.h"
#include "Engine/Render/Private/PrimitiveManager.h"
#include <queue>

namespace yjw
{
    RenderEntity::RenderEntity(RenderModule* render_module)
    {
        m_render_module = render_module;
    }

    RenderEntity::~RenderEntity()
    {
        ClearDrawItems();
    }

    void RenderEntity::UpdateMesh(MeshHandle handle)
    {
        m_mesh = handle;
        BuildDrawItems();
    }

    void RenderEntity::UpdateOverrideMaterial(const std::string& slot, MaterialHandle handle)
    {
        m_override_materials[slot] = handle;
        BuildDrawItems();
    }

    void RenderEntity::ClearDrawItems()
    {
        for (DrawItem& item : m_draw_items)
        {
            item.m_pipeline->release();
            item.m_primitive_binding.Release();
            item.m_resource_binding.Release();
        }
        m_draw_items.clear();
    }

    void RenderEntity::BuildDrawItems()
    {
        ClearDrawItems();
        Primitive* primitive = m_render_module->m_primitive_manager->GetMesh(m_mesh);
        if (!primitive)
        {
            return;
        }
        for (const SubPrimitive& sub_primitive : primitive->GetSubPrimitive())
        {
            DrawItem item{};
            item.m_sub_primitive_id = sub_primitive.m_sub_primitive_id;
            item.m_primitive_binding = primitive->GetPrimitiveBinding();
            item.m_primitive_binding.Retain();
            MaterialInstance* override_material = GetOverrideMaterial(sub_primitive.m_material_slot);
            if (override_material)
            {
                rpi::RPIRenderPipelineDescriptor pipelineDesc = rpi::RPIGetDefaultRenderPipeline();
                pipelineDesc.vs = primitive->GetVertexShader();
                pipelineDesc.ps = override_material->GetPixelShader();
                pipelineDesc.depth_stencil_state = rpi::RPIGetDepthStencilState(rpi::RPIDepthStencilStateType::default_depth_read_and_write);
                item.m_pipeline = rpi::RPICreateRenderPipeline(pipelineDesc);
                item.m_resource_binding = rpi::RPICreateResourceBinding();
                item.m_resource_binding.SetResourceSet(rpi::RPIResourceSetType::common, g_internal_shader_parameters.GetCommonResourceSet());
                item.m_resource_binding.SetResourceSet(rpi::RPIResourceSetType::vs, primitive->GetVSResourceSet());
                item.m_resource_binding.SetResourceSet(rpi::RPIResourceSetType::ps, override_material->GetResourceSet());
            }
            else
            {
                item.m_resource_binding = sub_primitive.m_resource_binding;
                item.m_pipeline = sub_primitive.m_pipeline;
                item.m_resource_binding.Retain();
                item.m_pipeline->retain(nullptr);
            }
            m_draw_items.push_back(item);
        }
    }

    MaterialInstance* RenderEntity::GetOverrideMaterial(const std::string& slot)
    {
        if (m_override_materials.find(slot) != m_override_materials.end())
        {
            return m_render_module->m_material_manager->GetMaterialInstance(m_override_materials[slot]);
        }
        return nullptr;
    }

    const std::vector<DrawItem>& RenderEntity::GetDrawItems()
    {
        return m_draw_items;
    }

    Scene::~Scene()
    {
        for (auto itr : m_entities)
        {
            if (itr.second)
            {
                delete itr.second;
            }
        }
        m_entities.clear();
    }

    EntityHandle Scene::AddEntity()
    {
        m_entity_id_allocator++;
        m_entities[m_entity_id_allocator] = new RenderEntity(m_render_module);
        return m_entity_id_allocator;
    }

    void Scene::EraseEntity(EntityHandle handle)
    {
        if (m_entities.find(handle) != m_entities.end())
        {
            delete m_entities[handle];
            m_entities.erase(handle);
        }
    }

    void Scene::UpdateEntityMesh(EntityHandle entity, MeshHandle mesh)
    {
        if (m_entities.find(entity) != m_entities.end())
        {
            m_entities[entity]->UpdateMesh(mesh);
        }
    }

    void Scene::UpdateEntityOverrideMaterial(EntityHandle entity, const std::string& slot, MaterialHandle material)
    {
        if (m_entities.find(entity) != m_entities.end())
        {
            m_entities[entity]->UpdateOverrideMaterial(slot, material);
        }
    }

    void Scene::GetDrawItems(std::vector<DrawItem>& v)
    {
        for (auto itr : m_entities)
        {
            if (itr.second)
            {
                v.insert(v.end(), itr.second->GetDrawItems().begin(), itr.second->GetDrawItems().end());
            }
        }
    }

    void Scene::Update(float deltaTime)
    {

    }

}