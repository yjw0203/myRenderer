#include "Engine/Render/Private/Scene.h"
#include "Engine/Engine/Public/Framework/Components/PrimitiveComponent.h"
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
#include "Engine/Render/Private/MaterialManager.h"
#include "Engine/Render/Private/PrimitiveManager.h"
#include <queue>

namespace yjw
{
    RdEntity::RdEntity(RdContext* context)
    {
        m_context = context;
    }

    RdEntity::~RdEntity()
    {
        ClearDrawItems();
    }

    void RdEntity::UpdateMesh(RdGeometryPtr handle)
    {
        m_mesh = handle;
        BuildDrawItems();
    }

    void RdEntity::UpdateOverrideMaterial(const std::string& slot, RdMaterialPtr handle)
    {
        m_override_materials[slot] = handle;
        BuildDrawItems();
    }

    void RdEntity::UpdatePickFlag(int pick_flag[4])
    {
        m_pick_flag[0] = pick_flag[0];
        m_pick_flag[1] = pick_flag[1];
        m_pick_flag[2] = pick_flag[2];
        m_pick_flag[3] = pick_flag[3];
    }

    int* RdEntity::GetPickFlag()
    {
        return m_pick_flag;
    }

    void RdEntity::UpdateRenderMask(RdRenderMaskBits maskBit, bool enable)
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

    bool RdEntity::GetRenderMask(RdRenderMaskBits maskBit)
    {
        return (m_render_mask & maskBit) != 0;
    }

    void RdEntity::ClearDrawItems()
    {
        m_draw_items.clear();
    }

    void RdEntity::BuildDrawItems()
    {
        ClearDrawItems();
        RdGeometry* primitive = m_context->m_primitive_manager.GetMesh(m_mesh);
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
            RdMaterial* override_material = GetOverrideMaterial(sub_primitive.m_material_slot);
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

    RdMaterial* RdEntity::GetOverrideMaterial(const std::string& slot)
    {
        if (m_override_materials.find(slot) != m_override_materials.end())
        {
            return m_context->m_material_manager.GetMaterialInstance(m_override_materials[slot]);
        }
        return nullptr;
    }

    const std::vector<DrawItem>& RdEntity::GetDrawItems()
    {
        return m_draw_items;
    }

    RdScene::~RdScene()
    {
        for (auto itr : m_entities)
        {
            if (itr)
            {
                delete itr;
            }
        }
        m_entities.clear();
    }

    RdEntityPtr RdScene::AddEntity()
    {
        RdEntityPtr entity = new RdEntity(m_context);
        m_entities.insert(entity);
        return entity;
    }

    void RdScene::EraseEntity(RdEntityPtr handle)
    {
        if (m_entities.find(handle) != m_entities.end())
        {
            delete handle;
            m_entities.erase(handle);
        }
    }

    void RdScene::UpdateEntityMesh(RdEntityPtr entity, RdGeometryPtr mesh)
    {
        if (m_entities.find(entity) != m_entities.end())
        {
            entity->UpdateMesh(mesh);
        }
    }

    void RdScene::UpdateEntityOverrideMaterial(RdEntityPtr entity, const std::string& slot, RdMaterialPtr material)
    {
        if (m_entities.find(entity) != m_entities.end())
        {
            entity->UpdateOverrideMaterial(slot, material);
        }
    }

    void RdScene::UpdateEntityPickFlag(RdEntityPtr entity, int pick_flag[4])
    {
        if (m_entities.find(entity) != m_entities.end())
        {
            entity->UpdatePickFlag(pick_flag);
        }
    }

    void RdScene::UpdateEntityRenderMask(RdEntityPtr entity, RdRenderMaskBits maskBit, bool enable)
    {
        if (m_entities.find(entity) != m_entities.end())
        {
            entity->UpdateRenderMask(maskBit, enable);
        }
    }

    void RdScene::GetDrawItems(std::vector<DrawItem>& v)
    {
        for (auto itr : m_entities)
        {
            if (itr)
            {
                v.insert(v.end(), itr->GetDrawItems().begin(), itr->GetDrawItems().end());
            }
        }
    }

    void RdScene::Update(float deltaTime)
    {

    }

}