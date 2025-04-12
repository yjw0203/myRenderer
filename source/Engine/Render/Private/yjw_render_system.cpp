#pragma once

#include "Engine/Render/Public/yjw_render_system.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "projectInfo.h"
#include "glm/glm.hpp"
#include "yjw_render_camera.h"
#include <chrono>
#include <ctime>
#include "Engine/Model/Public/yjw_model.h"

#include "Engine/InternalShaderResource/Public/yjw_internal_shader_resource.h"
#include "Engine/Animation/Public/Skeleton.h"
#include "Engine/Model/Public/AnimationLoader.h"

#include "Engine/Render/Private/Renderer/ForwardRenderer.h"

namespace yjw
{
    using namespace rpi;

    void RenderSystem::initialize()
    {
        activeCamera = new RenderCamera();
        RPIInit();

        g_internal_shader_parameters.Initialize();
        g_internal_shader_parameters.m_light->lightDirection = glm::vec3(-3, 15, -8);
        g_internal_shader_parameters.m_light->lightColor = glm::vec3(5, 5, 5);
        g_internal_shader_parameters.m_option->screenSize = glm::vec2(1200, 1200);

        activeCamera->SetPosition(glm::vec3(7.9, 14, -13));
        //activeCamera->SetRotation(glm::quat(0.2, -0.93, 0.04, -0.3));
        activeCamera->SetRotation(glm::quat(0,0,0,1));

        m_camera_dispatcher = new RenderCameraInputDispatcher(this);
        m_camera_dispatcher->Register();
    }
    
    void RenderSystem::tick(float deltaTime)
    {
        m_delta_time = deltaTime;
        g_internal_shader_parameters.m_camera->viewMat = activeCamera->getViewMatrix();
        g_internal_shader_parameters.m_camera->viewMatWithoutTranslation = activeCamera->getViewMatrixWithoutTranslation();
        g_internal_shader_parameters.m_camera->projectMat = activeCamera->getProjectionMatrix();
        g_internal_shader_parameters.m_camera->cameraPos = glm::vec4(activeCamera->position(), 1);

        g_internal_shader_parameters.FlushCpuDataToGpu();

    }
    void RenderSystem::shutdown()
    {
    }

}