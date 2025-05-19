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

    struct alignas(16) RdEntityData
    {
        RdEntityData() { m_model_matrix = Transform().getMatrix(); }
        Matrix4x4 m_model_matrix{};
        int skeletal_start_id{0};
    };

    class RdEntity
    {
    public:
        RdEntity(RdContext* context);
        ~RdEntity();

        int GetDataId();
        void UpdateDataId(int id);
        void UpdateMesh(RdGeometryPtr handle);
        void UpdateOverrideMaterial(const std::string& slot, RdMaterialPtr handle);
        void UpdateEntityDataID(int id);
        void UpdatePickFlag(int pick_flag);
        int* GetPushContants();
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

        int m_data_id{};
        int m_push_constants[4] = {}; //0: entity_data_id, 3: pick_flag
        int m_render_mask{};
    };

    class PrimitiveComponent;
    class RdScene
    {
    public:
        RdScene(RdContext* context);
        ~RdScene();

        static void OnInit();

        RdEntityPtr AddEntity();
        void EraseEntity(RdEntityPtr handle);
        void UpdateEntityTransform(RdEntityPtr entity, Transform transform);
        void UpdateEntitySkeletalMatrix(RdEntityPtr entity, const Matrix4x4* data, int count);
        void UpdateEntityMesh(RdEntityPtr entity, RdGeometryPtr mesh);
        void UpdateEntityOverrideMaterial(RdEntityPtr entity, const std::string& slot, RdMaterialPtr material);
        void UpdateEntityPickFlag(RdEntityPtr entity, int pick_flag);
        void UpdateEntityRenderMask(RdEntityPtr entity, RdRenderMaskBits maskBit, bool enable);

        void GetDrawItems(std::vector<DrawItem>& v);
        rpi::RPIResourceSet GetEntityResourceSet();

        void resetSkeletal();

        void Update();
    private:
        std::set<RdEntity*> m_entities;
        RdContext* m_context{nullptr};

        rpi::RPIResourceSet m_entity_resource_set{};
        rpi::RPIBuffer m_entity_data_buffer{};
        std::vector<RdEntityData> m_entity_data{};
        std::vector<int> m_entity_data_free{};

        rpi::RPIBuffer m_skeletal_matrix_buffer{};
        std::vector<Matrix4x4> m_skeletal_matrix_data{};
    };
}
