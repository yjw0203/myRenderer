#pragma once
#include "Engine/Engine/Public/Framework/Render/SceneInterface.h"
#include "Engine/Render/Public/RenderModule.h"
#include "Engine/RHI/Public/rpi/yjw_rpi.h"
#include "Engine/Render/Private/Primitive.h"
#include <vector>

namespace yjw
{
    class RenderModule;
    class MaterialInstance;

    struct DrawItem
    {
        int m_sub_primitive_id{};
        Primitive* m_primitive{};
        MaterialInstance* m_material{};
    };

    class RenderEntity
    {
    public:
        RenderEntity(RenderModule* render_module);
        ~RenderEntity();
        void UpdateMesh(MeshHandle handle);
        void UpdateOverrideMaterial(const std::string& slot, MaterialHandle handle);
        void ClearDrawItems();
        void BuildDrawItems();
        MaterialInstance* GetOverrideMaterial(const std::string& slot);
        
        const std::vector<DrawItem>& GetDrawItems();
    private:
        MeshHandle m_mesh{};
        std::unordered_map<std::string, MaterialHandle> m_override_materials;

        std::vector<DrawItem> m_draw_items;

        RenderModule* m_render_module{ nullptr };
    };

    class PrimitiveComponent;
    class Scene : public SceneInterface
    {
    public:
        Scene(RenderModule* render_module) : m_render_module{render_module} {};
        ~Scene();

        virtual EntityHandle AddEntity() override;
        virtual void EraseEntity(EntityHandle handle) override;
        virtual void UpdateEntityMesh(EntityHandle entity, MeshHandle mesh) override;
        virtual void UpdateEntityOverrideMaterial(EntityHandle entity, const std::string& slot, MaterialHandle material) override;

        void GetDrawItems(std::vector<DrawItem>& v);

        void Update(float deltaTime);
    private:
        EntityHandle m_entity_id_allocator = 0;
        std::unordered_map<EntityHandle, RenderEntity*> m_entities;

        RenderModule* m_render_module{nullptr};
    };
}
