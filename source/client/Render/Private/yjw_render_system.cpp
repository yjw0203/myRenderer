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
        
        //Cube = *Model::load(RESOURCE_FILE(Cube),"Cube.gltf", model_file_format_gltf);
        //saibo = *Model::load(RESOURCE_FILE(saibo),"赛博美女.glb", model_file_format_glb);
        naxita = *Model::load(RESOURCE_FILE(cao), "纳西妲.pmx", model_file_format_pmx);
        //heita = *Model::load(RESOURCE_FILE(heita),"黑塔.pmx", model_file_format_pmx);
        //hutao = *Model::load(RESOURCE_FILE(hutao),"胡桃.pmx", model_file_format_pmx);

        naxita_sk = std::make_shared<Skeleton>();
        naxita_sk->BuildSkeleton(naxita->m_cpu_model->m_skeleton_data);

        naxita_anim.Load(RESOURCE_FILE(cao), "动作.vmd");

        naxita_sk->LoadController(naxita_anim.m_animation->CreateSkeletonController());

        scene->models.push_back(naxita);
        //scene.models.push_back(heita);
        //scene.models.push_back(hutao);
        //scene->models.push_back(Cube);
        //scene->models.push_back(saibo);

        activeCamera->SetPosition(glm::vec3(7.9, 14, -13));
        activeCamera->SetRotation(glm::quat(0.2, -0.93, 0.04, -0.3));
        //activeCamera.direction = glm::vec3(0, 0, 0) - activeCamera.position;

        pipeline.initializeResource();
        DefaultPipelineConfig config;
        config.window = rpiWindow;
        pipeline.config(config);

    }
    
    void RenderSystem::tick()
    {
        static time_t currentRealTime = 0;
        static float currentTime = 0;

        std::chrono::system_clock::time_point time_point_now = std::chrono::system_clock::now();
        std::chrono::system_clock::duration duration_since_epoch = time_point_now.time_since_epoch();
        time_t microseconds_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(duration_since_epoch).count();
        time_t time = microseconds_since_epoch;
        time_t delta_time_micro = time - currentRealTime;
        if (currentRealTime == 0)
        {
            delta_time_micro = 0;
        }
        deltaTime = delta_time_micro / 1000000.0f;
        currentRealTime = time;

        naxita_sk->Update(currentTime);

        pipeline.forwardPass->setBoneMatrices(naxita_sk->GetBoneMatrices());

        pipeline.ClearDebugLine();
        for (const Bone& bone : naxita_sk->GetBones())
        {
            if (bone.m_parent)
            {
                //pipeline.AddDebugLine(bone.m_pose.m_location, bone.m_parent->m_pose.m_location);
            }
        }

        RenderCamera& camera = *RenderSystem::get().activeCamera;
        g_internal_shader_parameters.m_camera->viewMat = camera.getViewMatrix();
        g_internal_shader_parameters.m_camera->projectMat = camera.getProjectionMatrix();
        g_internal_shader_parameters.m_camera->cameraPos = glm::vec4(camera.position(), 1);

        g_internal_shader_parameters.FlushCpuDataToGpu();
        pipeline.render();
        rpi::RPIPresent(pipeline.commandBuffer, rpiWindow, pipeline.output);
        WindowsManager::get().loop();

        currentTime += deltaTime;
    }
    void RenderSystem::shutdown()
    {
        WindowsManager::get().shutdown();

    }

}