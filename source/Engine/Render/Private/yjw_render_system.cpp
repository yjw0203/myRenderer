#pragma once

#include "Engine/Render/Public/yjw_render_system.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/Render/Private/yjw_windows_manager.h"
#include "projectInfo.h"
#include "glm/glm.hpp"
#include "yjw_render_camera.h"
#include <chrono>
#include <ctime>
#include "pass/yjw_pass_gbuffer.h"
#include "pass/yjw_pass_deferred_shading.h"
#include "pipeline/yjw_default_pipeline.h"

#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
#include "Engine/Animation/Public/Skeleton.h"
#include "Engine/Model/Public/AnimationLoader.h"

#include "Engine/Render/Private/Renderer/ForwardRenderer.h"

namespace yjw
{
    using namespace rpi;

    std::shared_ptr<Model> naxita;
    std::shared_ptr<Skeleton> naxita_sk;
    std::shared_ptr<Model> heita;
    std::shared_ptr<Model> hutao;
    std::shared_ptr<Model> Cube;
    std::shared_ptr<Model> saibo;

    VMD naxita_anim;

    RPIContext g_context;

    ForwardRenderer* renderer = new ForwardRenderer();

    void RenderSystem::initialize()
    {
        activeCamera = new RenderCamera();
        Window::Initialize();
        RPIInit();
        
        m_window = new Window();

        g_internal_shader_parameters.Initialize();
        g_internal_shader_parameters.m_light->lightDirection = glm::vec3(-3, 15, -8);
        g_internal_shader_parameters.m_light->lightColor = glm::vec3(5, 5, 5);
        g_internal_shader_parameters.m_option->screenSize = glm::vec2(1200, 1200);

        activeCamera->SetPosition(glm::vec3(7.9, 14, -13));
        activeCamera->SetRotation(glm::quat(0.2, -0.93, 0.04, -0.3));

        m_camera_dispatcher = new RenderCameraInputDispatcher(this);
        m_camera_dispatcher->Register();

        renderer->Initialize();

        g_context = RPICreateContext();
    }
    
    void RenderSystem::tick(float deltaTime)
    {
        m_delta_time = deltaTime;
        g_internal_shader_parameters.m_camera->viewMat = activeCamera->getViewMatrix();
        g_internal_shader_parameters.m_camera->projectMat = activeCamera->getProjectionMatrix();
        g_internal_shader_parameters.m_camera->cameraPos = glm::vec4(activeCamera->position(), 1);

        g_internal_shader_parameters.FlushCpuDataToGpu();

        //pipeline.render();
        //m_window->Present(pipeline.commandBuffer);
        RPICmdClearBackBuffer(g_context, m_window->GetRPIWindow());
        RPISubmit(g_context);

        renderer->SetRenderPass(m_window->GetRPIWindow().swapchain->GetCurrentRenderPass());
        renderer->SetSceneProxy(RenderSceneProxy(m_scene));

        renderer->BeginFrame();
        renderer->RenderFrame();
        renderer->EndFrame();

        m_window->Present(g_context);

        Window::PoolEvents();
    }
    void RenderSystem::shutdown()
    {
        renderer->Destroy();
        Window::Shutdown();
    }

    void RenderSystem::AttachScene(Scene* scene)
    {
        m_scene = scene;
    }

    void RenderSystem::AttachUI(rhi::ImGuiUI* ui)
    {
        m_ui = ui;
    }

}