#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi.h"
#include "Engine/Render/Private/Primitive.h"
#include "Engine/Render/Private/Context.h"
#include "Engine/Render/Public/RenderAPI.h"
#include <vector>
#include <set>

namespace yjw
{
    class RenderModule;
    class RdMaterial;

    struct DrawItem
    {
        class RdEntity* m_entity{};
        int m_sub_primitive_id{};
        RdGeometry* m_primitive{};
        RdMaterial* m_material{};
    };

    class RdEntity
    {
    public:
        RdEntity(RdContext* context);
        ~RdEntity();
        void UpdateMesh(RdGeometryPtr handle);
        void UpdateOverrideMaterial(const std::string& slot, RdMaterialPtr handle);
        void UpdatePickFlag(int pick_flag[4]);
        int* GetPickFlag();
        void UpdateRenderMask(RdRenderMaskBits maskBit, bool enable);
        bool GetRenderMask(RdRenderMaskBits maskBit);
        void ClearDrawItems();
        void BuildDrawItems();
        RdMaterial* GetOverrideMaterial(const std::string& slot);
        
        const std::vector<DrawItem>& GetDrawItems();
    private:
        RdGeometryPtr m_mesh{};
        std::unordered_map<std::string, RdMaterialPtr> m_override_materials;

        std::vector<DrawItem> m_draw_items;

        RdContext* m_context{ nullptr };

        int m_pick_flag[4] = {};
        int m_render_mask{};
    };

    class PrimitiveComponent;
    class RdScene
    {
    public:
        RdScene(RdContext* context) : m_context{ context } {};
        ~RdScene();

        RdEntityPtr AddEntity();
        void EraseEntity(RdEntityPtr handle);
        void UpdateEntityMesh(RdEntityPtr entity, RdGeometryPtr mesh);
        void UpdateEntityOverrideMaterial(RdEntityPtr entity, const std::string& slot, RdMaterialPtr material);
        void UpdateEntityPickFlag(RdEntityPtr entity, int pick_flag[4]);
        void UpdateEntityRenderMask(RdEntityPtr entity, RdRenderMaskBits maskBit, bool enable);

        void GetDrawItems(std::vector<DrawItem>& v);

        void Update(float deltaTime);
    private:
        RdEntityPtr m_entity_id_allocator = 0;
        std::set<RdEntity*> m_entities;

        RdContext* m_context{nullptr};
    };
}
