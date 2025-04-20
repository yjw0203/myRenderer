#pragma once
#include <vector>
#include "Engine/Math/Public/Transform.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"

#define RD_DEFINE_HANDLE(object) typedef class object *object##Ptr;

namespace yjw
{
    using namespace rhi;
    typedef class ImGuiUI* RdUIPtr;
    RD_DEFINE_HANDLE(RdScene);
    RD_DEFINE_HANDLE(RdEntity);
    RD_DEFINE_HANDLE(RdView);
    RD_DEFINE_HANDLE(RdMaterial);
    RD_DEFINE_HANDLE(RdGeometry);

    enum RdRenderMaskBits
    {
        highlight = 1 << 0
    };

    using RdRenderMask = int;
    
    typedef struct RdHitRequestStruct {
        int m_hit_flag{};
        int m_posx{};
        int m_posy{};
        bool m_completed = false;
        int m_result{};
    } RdHitRequestStruct;

    void rdInit();
    void rdTick(float delta);
    void rdDestroy();

    void rdDrawView(RdViewPtr view);

    RdViewPtr rdCreateView(void* window);
    void rdDestroyView(RdViewPtr view);
    
    RdScenePtr rdCreateScene();
    void rdDestroyScene(RdScenePtr scene);

    RdMaterialPtr rdCreateMaterial(const char* url);
    void rdDestroyMaterial(RdMaterialPtr material);

    RdGeometryPtr rdCreateGeometry(const char* url);
    void rdDestroyGeometry(RdGeometryPtr geometry);

    void rdAttachScene(RdViewPtr view, RdScenePtr scene);
    void rdAttachUI(RdViewPtr view, RdUIPtr ui);

    RdEntityPtr rdAddEntity(RdScenePtr scene);
    void rdRemoveEntity(RdScenePtr scene, RdEntityPtr entity);
    void rdUpdateEntityTransform(RdScenePtr scene, RdEntityPtr entity, const Transform& transform);
    void rdUpdateEntityOverrideMaterial(RdScenePtr scene, RdEntityPtr entity, const char* slot, RdMaterialPtr material);
    void rdUpdateEntityGeometry(RdScenePtr scene, RdEntityPtr entity, RdGeometryPtr geometry);
    void rdUpdateEntityPickFlag(RdScenePtr scene, RdEntityPtr entity, int pick_flag);
    void rdUpdateEntityRenderMask(RdScenePtr scene, RdEntityPtr entity, RdRenderMaskBits maskBit, bool enable);

    void rdAddPendingHitRequest(RdViewPtr view, const char* group_name, const RdHitRequestStruct& request);
    void rdGetProcessedHitRequest(RdViewPtr view, const char* group_name, std::vector<RdHitRequestStruct>& proccessed_request);

    glm::mat4x4 rdGetViewMatrix();
    glm::mat4x4 rdGetProjMatrix();

}