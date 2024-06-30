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

namespace yjw
{
    using namespace rpi;

    DefaultPipeline pipeline;

    std::shared_ptr<Model> naxita;
    std::shared_ptr<Model> heita;
    std::shared_ptr<Model> hutao;
    std::shared_ptr<Model> Cube;
    std::shared_ptr<Model> saibo;

    RPIWindow rpiWindow;

    void RenderSystem::initialize()
    {
        activeCamera = new RenderCamera();
        scene = new Scene();

        WindowsManager::get().initialize();
        RPIInit();
        rpiWindow = RPICreateWindow(WindowsManager::get().window);

        g_internal_shader_parameters.Initialize();
        g_internal_shader_parameters.m_light->lightDirection = glm::vec3(-3, 15, -8);
        g_internal_shader_parameters.m_light->lightColor = glm::vec3(5, 5, 5);
        g_internal_shader_parameters.m_option->screenSize = glm::vec2(1200, 1200);
        
        Cube = *Model::load(RESOURCE_FILE(Cube),"Cube.gltf", model_file_format_gltf);
        saibo = *Model::load(RESOURCE_FILE(saibo),"赛博美女.glb", model_file_format_glb);
        naxita = *Model::load(RESOURCE_FILE(cao), "纳西妲.pmx", model_file_format_pmx);
        heita = *Model::load(RESOURCE_FILE(heita),"黑塔.pmx", model_file_format_pmx);
        hutao = *Model::load(RESOURCE_FILE(hutao),"胡桃.pmx", model_file_format_pmx);

        scene->models.push_back(naxita);
        //scene.models.push_back(heita);
        //scene.models.push_back(hutao);
        //scene->models.push_back(Cube);
        //scene->models.push_back(saibo);

        activeCamera->position = glm::vec3(1, 12, -16);
        //activeCamera.direction = glm::vec3(0, 0, 0) - activeCamera.position;
        activeCamera->direction = glm::vec3(0, -0.2,1);
        activeCamera->up = glm::vec3(0, -1, -0.2);

        pipeline.initializeResource();
        DefaultPipelineConfig config;
        config.window = rpiWindow;
        pipeline.config(config);

    }
    
    void RenderSystem::tick()
    {
        static time_t currentTime = 0;

        std::chrono::system_clock::time_point time_point_now = std::chrono::system_clock::now();
        std::chrono::system_clock::duration duration_since_epoch = time_point_now.time_since_epoch();
        time_t microseconds_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(duration_since_epoch).count();
        time_t time = microseconds_since_epoch;
        deltaTime = (time - currentTime) / 1000000.0f;
        currentTime = time;

        RenderCamera& camera = *RenderSystem::get().activeCamera;
        g_internal_shader_parameters.m_camera->viewMat = camera.getViewMatrix();
        g_internal_shader_parameters.m_camera->projectMat = camera.getProjectionMatrix();
        g_internal_shader_parameters.m_camera->cameraPos = glm::vec4(camera.position, 1);

        g_internal_shader_parameters.FlushCpuDataToGpu();
        pipeline.render();
        rpi::RPIPresent(pipeline.commandBuffer, rpiWindow, pipeline.output);
        WindowsManager::get().loop();

    }
    void RenderSystem::shutdown()
    {
        WindowsManager::get().shutdown();

    }

}