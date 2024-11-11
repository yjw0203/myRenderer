#pragma once

#include "Render/yjw_render_system.h"
#include "rhi/rpi/yjw_rpi_header.h"
#include "Private/yjw_windows_manager.h"
#include "projectInfo.h"
#include "glm/glm.hpp"
#include "yjw_render_camera.h"
#include "yjw_scene.h"
#include <chrono>
#include <ctime>
#include "pass/yjw_pass_gbuffer.h"
#include "pass/yjw_pass_deferred_shading.h"
#include "pipeline/yjw_default_pipeline.h"

#include "InternalShaderResource/yjw_internal_shader_resource.h"
#include "Animation/Skeleton.h"
#include "Model/AnimationLoader.h"

namespace yjw
{
    using namespace rpi;

    DefaultPipeline pipeline;

    std::shared_ptr<Model> naxita;
    std::shared_ptr<Skeleton> naxita_sk;
    std::shared_ptr<Model> heita;
    std::shared_ptr<Model> hutao;
    std::shared_ptr<Model> Cube;
    std::shared_ptr<Model> saibo;

    VMD naxita_anim;

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

        pipeline.AttachScene(m_scene);
        pipeline.AttachUI(m_ui);
        pipeline.initializeResource();
        DefaultPipelineConfig config;
        config.window = m_window->GetRPIWindow();
        pipeline.config(config);

        m_camera_dispatcher = new RenderCameraInputDispatcher(this);
        m_camera_dispatcher->Register();

    }
    
    void RenderSystem::tick(float deltaTime)
    {
        m_delta_time = deltaTime;
        g_internal_shader_parameters.m_camera->viewMat = activeCamera->getViewMatrix();
        g_internal_shader_parameters.m_camera->projectMat = activeCamera->getProjectionMatrix();
        g_internal_shader_parameters.m_camera->cameraPos = glm::vec4(activeCamera->position(), 1);

        g_internal_shader_parameters.FlushCpuDataToGpu();

        pipeline.render();
        m_window->Present(pipeline.commandBuffer);

        Window::PoolEvents();
    }
    void RenderSystem::shutdown()
    {
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

    rpi::RPITexture RenderSystem::GetSceneTexture()
    {
        return pipeline.m_scene_texture;
    }

}