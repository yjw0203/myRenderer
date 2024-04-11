#pragma once

#include "yjw_render_system.h"
#include "rhi/rpi/yjw_rpi_header.h"
#include "client/WindowManager/yjw_windows_manager.h"
#include "generate/projectInfo.h"
#include "glm/glm.hpp"
#include "yjw_render_camera.h"
#include "yjw_scene.h"
#include <chrono>
#include <ctime>
#include "pass/yjw_pass_gbuffer.h"
#include "pass/yjw_pass_deferred_shading.h"
#include "yjw_resource.h"
#include "pipeline/yjw_default_pipeline.h"

namespace yjw
{
    using namespace rpi;

    DefaultPipeline pipeline;

    std::shared_ptr<Model> naxita;
    std::shared_ptr<Model> heita;
    std::shared_ptr<Model> hutao;

    RPIWindow rpiWindow;

    void RenderSystem::initialize()
    {
        WindowsManager::get().initialize();
        RPIInit();
        rpiWindow = RPICreateWindow(WindowsManager::get().window);
        g_resource_store.initializeResource();
        
        naxita = *Model::load(RESOURCE_FILE(cao),"纳西妲.pmx");
        heita = *Model::load(RESOURCE_FILE(heita),"黑塔.pmx");
        hutao = *Model::load(RESOURCE_FILE(hutao),"胡桃.pmx");

        //scene.models.push_back(naxita);
        //scene.models.push_back(heita);
        scene.models.push_back(hutao);

        activeCamera.position = glm::vec3(1, 12, -16);
        //activeCamera.direction = glm::vec3(0, 0, 0) - activeCamera.position;
        activeCamera.direction = glm::vec3(0, -0.2,1);
        activeCamera.up = glm::vec3(0, -1, -0.2);

        pipeline.initializeResource();
        pipeline.config();

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

        g_resource_store.updateCameraData();
        g_resource_store.updateLightData();

        pipeline.render();
        rpi::RPIPresent(pipeline.commandBuffer, rpiWindow, pipeline.output);
        WindowsManager::get().loop();

    }
    void RenderSystem::shutdown()
    {
        WindowsManager::get().shutdown();

    }

    REGISTER_DELEGATE(OnApplicationInitializedDelegate, []() {RenderSystem::get().initialize(); })
    REGISTER_DELEGATE(OnApplicationLoopDelegate, []() {RenderSystem::get().tick(); })
    REGISTER_DELEGATE(OnApplicationShutdownDelegate, []() {RenderSystem::get().shutdown(); })
}