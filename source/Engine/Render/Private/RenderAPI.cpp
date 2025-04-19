#include "Engine/Render/Public/RenderAPI.h"
#include "Engine/Render/Private/MaterialManager.h"
#include "Engine/Render/Private/PrimitiveManager.h"
#include "Engine/Render/Private/View.h"
#include "Engine/Render/Private/Context.h"
#include "Engine/Render/Private/yjw_render_camera.h"
#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
#include "glm/glm.hpp"

namespace yjw
{
    using namespace rpi;

    RdContext* g_rd_context = nullptr;

    void rdInit()
    {
        g_rd_context = new RdContext();
        g_rd_context->m_render_camera = new RenderCamera();
        RPIInit();

        g_internal_shader_parameters.Initialize();
        g_internal_shader_parameters.m_light->lightDirection = glm::vec3(-3, 15, -8);
        g_internal_shader_parameters.m_light->lightColor = glm::vec3(5, 5, 5);
        g_internal_shader_parameters.m_option->screenSize = glm::vec2(1200, 1200);

        g_rd_context->m_render_camera->SetPosition(glm::vec3(7.9, 14, -13));
        //activeCamera->SetRotation(glm::quat(0.2, -0.93, 0.04, -0.3));
        g_rd_context->m_render_camera->SetRotation(glm::quat(0, 0, 0, 1));
        (new RenderCameraInputDispatcher(g_rd_context))->Register();

        RdScene::OnInit();
    }

    void rdTick(float delta)
    {
        g_rd_context->m_delta_time = delta;
        g_internal_shader_parameters.m_camera->viewMat = g_rd_context->m_render_camera->getViewMatrix();
        g_internal_shader_parameters.m_camera->viewMatWithoutTranslation = g_rd_context->m_render_camera->getViewMatrixWithoutTranslation();
        g_internal_shader_parameters.m_camera->projectMat = g_rd_context->m_render_camera->getProjectionMatrix();
        g_internal_shader_parameters.m_camera->cameraPos = glm::vec4(g_rd_context->m_render_camera->position(), 1);
        g_internal_shader_parameters.m_camera->near_far = glm::vec2(g_rd_context->m_render_camera->near(), g_rd_context->m_render_camera->far());

        g_internal_shader_parameters.FlushCpuDataToGpu();
    }

    void rdDestroy()
    {
        delete g_rd_context;
    }

    void rdDrawView(RdViewPtr view)
    {
        view->Draw();
    }

    RdViewPtr rdCreateView(void* window)
    {
        return new RdView(window);
    }

    void rdDestroyView(RdViewPtr view)
    {
        delete view;
    }

    RdScenePtr rdCreateScene()
    {
        return new RdScene(g_rd_context);
    }

    void rdDestroyScene(RdScenePtr scene)
    {
        delete scene;
    }

    RdMaterialPtr rdCreateMaterial(const char* url)
    {
        return g_rd_context->m_material_manager.LoadMaterial(url);
    }

    void rdDestroyMaterial(RdMaterialPtr material)
    {
        g_rd_context->m_material_manager.UnloadMaterial(material);
    }

    RdGeometryPtr rdCreateGeometry(const char* url)
    {
        return g_rd_context->m_primitive_manager.LoadMesh(url);
    }

    void rdDestroyGeometry(RdGeometryPtr geometry)
    {
        g_rd_context->m_primitive_manager.UnloadMesh(geometry);
    }

    void rdAttachScene(RdViewPtr view, RdScenePtr scene)
    {
        view->AttachScene(scene);
    }

    void rdAttachUI(RdViewPtr view, RdUIPtr ui)
    {
        view->AttachUI(ui);
    }

    RdEntityPtr rdAddEntity(RdScenePtr scene)
    {
        return scene->AddEntity();
    }

    void rdRemoveEntity(RdScenePtr scene, RdEntityPtr entity)
    {
        scene->EraseEntity(entity);
    }

    void rdUpdateEntityTransform(RdScenePtr scene, RdEntityPtr entity, const Transform& transform)
    {
        scene->UpdateEntityTransform(entity, transform);
    }

    void rdUpdateEntityOverrideMaterial(RdScenePtr scene, RdEntityPtr entity, const char* slot, RdMaterialPtr material)
    {
        scene->UpdateEntityOverrideMaterial(entity, slot, material);
    }

    void rdUpdateEntityGeometry(RdScenePtr scene, RdEntityPtr entity, RdGeometryPtr geometry)
    {
        scene->UpdateEntityMesh(entity, geometry);
    }

    void rdUpdateEntityPickFlag(RdScenePtr scene, RdEntityPtr entity, int pick_flag)
    {
        scene->UpdateEntityPickFlag(entity, pick_flag);
    }

    void rdUpdateEntityRenderMask(RdScenePtr scene, RdEntityPtr entity, RdRenderMaskBits maskBit, bool enable)
    {
        scene->UpdateEntityRenderMask(entity, maskBit, enable);
    }

    void rdAddPendingHitRequest(RdViewPtr view, const char* group_name, const RdHitRequestStruct& request)
    {
        view->AddPendingHitRequest(group_name, request);
    }

    void rdGetProcessedHitRequest(RdViewPtr view, const char* group_name, std::vector<RdHitRequestStruct>& proccessed_request)
    {
        view->GetProcessedHitRequest(group_name, proccessed_request);
    }
}