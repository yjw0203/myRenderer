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

    int RdEntity::GetDataId()
    {
        return m_data_id;
    }

    void RdEntity::UpdateDataId(int id)
    {
        m_data_id = id;
        m_push_constants[0] = id;
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

    void RdEntity::UpdateEntityDataID(int id)
    {
        m_push_constants[0] = id;
    }

    void RdEntity::UpdatePickFlag(int pick_flag)
    {
        m_push_constants[3] = pick_flag;
    }

    int* RdEntity::GetPushContants()
    {
        return m_push_constants;
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

    RdScene::RdScene(RdContext* context)
        :m_context(context)
    {
        rpi::ShaderReflect reflect{};
        rpi::ShaderReflect::SSBO buffer{};
        buffer.m_set = rpi::RPIGetResourceSetIDByType(rpi::RPIResourceSetType::entity);
        buffer.m_name = "EntityDataBuffer";
        buffer.m_binding = 0;
        reflect.m_ssbos.push_back(buffer);
        buffer.m_set = rpi::RPIGetResourceSetIDByType(rpi::RPIResourceSetType::entity);
        buffer.m_name = "SkeletalMatrixBuffer";
        buffer.m_binding = 1;
        reflect.m_ssbos.push_back(buffer);
        m_entity_resource_set = rpi::RPICreateResourceSet(rpi::RPIResourceSetType::entity, &reflect);

        int max_entity = 1000; // to be remove
        m_entity_data_buffer = rpi::RPICreateUploadBuffer(max_entity * sizeof(RdEntityData));
        m_entity_resource_set.SetBuffer("EntityDataBuffer", m_entity_data_buffer);

        int max_bone = 100000;
        m_skeletal_matrix_buffer = rpi::RPICreateUploadBuffer(max_bone * sizeof(Matrix4x4));
        m_entity_resource_set.SetBuffer("SkeletalMatrixBuffer", m_skeletal_matrix_buffer);
    }

    void RdScene::OnInit()
    {
        rpi::ShaderReflect reflect{};
        rpi::ShaderReflect::SSBO buffer{};
        buffer.m_set = rpi::RPIGetResourceSetIDByType(rpi::RPIResourceSetType::entity);
        buffer.m_name = "EntityDataBuffer";
        buffer.m_binding = 0;
        reflect.m_ssbos.push_back(buffer);
        buffer.m_set = rpi::RPIGetResourceSetIDByType(rpi::RPIResourceSetType::entity);
        buffer.m_name = "SkeletalMatrixBuffer";
        buffer.m_binding = 1;
        reflect.m_ssbos.push_back(buffer);
        rpi::RPIGlobalSetResourceSetLayout(rpi::RPIResourceSetType::entity, &reflect);
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
        int id = -1;
        for (int i = 0; i < m_entity_data.size(); i++)
        {
            if (m_entity_data_free[i])
            {
                id = i;
                m_entity_data[i] = RdEntityData{};
                m_entity_data_free[i] = 0;
                break;
            }
        }
        if (id == -1)
        {
            id = m_entity_data.size();
            m_entity_data.push_back(RdEntityData{});
            m_entity_data_free.push_back(0);
        }
        entity->UpdateDataId(id);
        return entity;
    }

    void RdScene::EraseEntity(RdEntityPtr handle)
    {
        if (m_entities.find(handle) != m_entities.end())
        {
            m_entity_data_free[handle->GetDataId()] = 1;
            delete handle;
            m_entities.erase(handle);
        }
    }

    void RdScene::UpdateEntityTransform(RdEntityPtr entity, Transform transform)
    {
        m_entity_data[entity->GetDataId()].m_model_matrix = transform.getMatrix();
    }

    void RdScene::UpdateEntitySkeletalMatrix(RdEntityPtr entity, const Matrix4x4* data, int count)
    {
        int old_size = m_skeletal_matrix_data.size();
        m_skeletal_matrix_data.resize(old_size + count);
        memcpy(&m_skeletal_matrix_data[old_size], data, sizeof(Matrix4x4) * count);
        m_entity_data[entity->GetDataId()].skeletal_start_id = old_size;
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

    void RdScene::UpdateEntityPickFlag(RdEntityPtr entity, int pick_flag)
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

    rpi::RPIResourceSet RdScene::GetEntityResourceSet()
    {
        return m_entity_resource_set;
    }

    void RdScene::resetSkeletal()
    {
        m_skeletal_matrix_data.resize(1);
        m_skeletal_matrix_data[0] = Matrix4x4(1.0f);
    }

    void RdScene::Update()
    {
        rpi::RPIUpdateBuffer(m_entity_data_buffer, m_entity_data.data(), 0, m_entity_data.size() * sizeof(RdEntityData));
        rpi::RPIUpdateBuffer(m_skeletal_matrix_buffer, m_skeletal_matrix_data.data(), 0, m_skeletal_matrix_data.size() * sizeof(Matrix4x4));
    }

}