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

    void RenderEntity::UpdatePickFlag(int pick_flag[4])
    {
        m_pick_flag[0] = pick_flag[0];
        m_pick_flag[1] = pick_flag[1];
        m_pick_flag[2] = pick_flag[2];
        m_pick_flag[3] = pick_flag[3];
    }

    int* RenderEntity::GetPickFlag()
    {
        return m_pick_flag;
    }

    void RenderEntity::UpdateRenderMask(RenderMaskBits maskBit, bool enable)
    {
        if (enable)
        {
            m_render_mask |= maskBit;
        }
        else
        {
            m_render_mask &= ~maskBit;
        }
    }

    bool RenderEntity::GetRenderMask(RenderMaskBits maskBit)
    {
        return (m_render_mask & maskBit) != 0;
    }

    void RenderEntity::ClearDrawItems()
    {
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
            item.m_entity = this;
            item.m_sub_primitive_id = sub_primitive.m_sub_primitive_id;
            item.m_primitive = primitive;
            MaterialInstance* override_material = GetOverrideMaterial(sub_primitive.m_material_slot);
            if (override_material)
            {
                item.m_material = override_material;
            }
            else
            {
                item.m_material = sub_primitive.m_material;
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

    void Scene::UpdateEntityPickFlag(EntityHandle entity, int pick_flag[4])
    {
        if (m_entities.find(entity) != m_entities.end())
        {
            m_entities[entity]->UpdatePickFlag(pick_flag);
        }
    }

    void Scene::UpdateEntityRenderMask(EntityHandle entity, RenderMaskBits maskBit, bool enable)
    {
        if (m_entities.find(entity) != m_entities.end())
        {
            m_entities[entity]->UpdateRenderMask(maskBit, enable);
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