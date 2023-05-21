#pragma once

#include "yjw_render_system.h"
#include "rhi/rhi/yjw_rhi_header.h"
#include "client/WindowManager/yjw_windows_manager.h"
#include "generate/projectInfo.h"
#include "glm/glm.hpp"
#include "yjw_render_camera.h"
#include "yjw_scene.h"
#include <chrono>
#include <ctime>

namespace yjw
{
    using namespace rhi;

    std::shared_ptr<RHIShader> vs = nullptr;
    std::shared_ptr<RHIShader> ps = nullptr;

    RHITexture2D* image = nullptr;
    RHITexture2D* depthImage = nullptr;
    std::shared_ptr<RHIColorAttachment> color_attachment;
    std::shared_ptr<RHIDepthStencilAttachment> depth_attachment;
    std::vector<std::shared_ptr<RHIDescriptorsSet>> descriptors_sets;

    std::shared_ptr<RHIUniformBuffer> camera_uniform;
    std::shared_ptr<RHIUniformBuffer> camera_pos_uniform;
    std::shared_ptr<RHIUniformBuffer> light_uniform;

    std::shared_ptr<Model> naxita;
    std::shared_ptr<Model> heita;
    std::shared_ptr<Model> hutao;

    struct Vex
    {
        glm::vec3 pos;
        glm::vec2 uv;
    };
    Vex vex[4] = {
        {glm::vec3(-0.1,-0.1,0.1),glm::vec2{1,0}},
        {glm::vec3(0.1,-0.1,0.1),glm::vec2{0,0}},
        {glm::vec3(0.1,0.1,0.1),glm::vec2{0,1}},
        {glm::vec3(-0.1,0.1,0.1),glm::vec2{1,1}},
    };

    uint32_t index[6] = { 0,1,2,2,3,0 };

    std::shared_ptr<RHIPipeline> pipeline;
    std::shared_ptr<RHIAttachmentsSet> attachementSet;
    std::shared_ptr<RHIDescriptorsSet> descriptorSet;

    void RenderSystem::initialize()
    {
        WindowsManager::get().initialize();
        CreateInfo rhi_createinfo{};
        rhi_createinfo.window = WindowsManager::get().window;
        IRHI::Get()->initialize(rhi_createinfo);

        naxita = *Model::load(RESOURCE_FILE(cao),"纳西妲.pmx");
        heita = *Model::load(RESOURCE_FILE(heita),"黑塔.pmx");
        hutao = *Model::load(RESOURCE_FILE(hutao),"胡桃.pmx");

        scene.models.push_back(naxita);
        scene.models.push_back(heita);
        scene.models.push_back(hutao);

        activeCamera.position = glm::vec3(9, -10, 0);
        //activeCamera.direction = glm::vec3(0, 0, 0) - activeCamera.position;
        activeCamera.direction = glm::vec3(0, 0, 1);
        activeCamera.up = glm::vec3(0, -1, 0);

        vs = std::make_shared<RHIShader>(SHADER_FILE(test_vert.spv));
        ps = std::make_shared<RHIShader>(SHADER_FILE(test_frag.spv));

        image = new RHITexture2D(720, 720, 1, RHIFormat::R8G8B8A8_unorm, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        IRHI::Get()->resourceBarrierImmidiately(image, RHIResourceState::undefine, RHIResourceState::render_target);
        
        depthImage = new RHITexture2D(720, 720, 1, RHIFormat::D24_unorm_S8_uint, RHIResourceUsageBits::allow_depth_stencil, RHIMemoryType::default_);
        IRHI::Get()->resourceBarrierImmidiately(depthImage, RHIResourceState::undefine, RHIResourceState::depth_stencil_write);

        color_attachment = std::make_shared<RHIColorAttachment>(image, RHIFormat::R8G8B8A8_unorm);
        depth_attachment = std::make_shared<RHIDepthStencilAttachment>(depthImage, RHIFormat::D24_unorm_S8_uint);

        camera_uniform = std::make_shared<RHIUniformBuffer>(128);
        light_uniform = std::make_shared<RHIUniformBuffer>(32);
        camera_pos_uniform = std::make_shared<RHIUniformBuffer>(16);

        pipeline = RHIPipeline::NewGraphicsPipline()
            .VS(vs, "main")
            .PS(ps, "main")
            .RASTERIZATION_STATE(Rasterization_default)
            .COLOR_BLEND_STATE(ColorBlend_default)
            .DEPTH_STENCIL_STATE(DepthStencil_default)
            .VERTEX_BINDING<0, 0>(R32G32B32_sfloat)
            .VERTEX_BINDING<0, 1>(R32G32B32_sfloat)
            .VERTEX_BINDING<0, 2>(R32G32_sfloat)
            .COLOR_ATTACHMENT<0>(R8G8B8A8_unorm)
            .DEETH_STENCIL_ATTACHMENT(D24_unorm_S8_uint)
            .UNIFORM_BUFFER<RHIShaderType::vertex_shader, 0, 0>(128)
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 1>(32)
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 2>(16)
            .TEXTURE_SRV<RHIShaderType::pixel_shader, 1, 0>();

        attachementSet = RHIAttachmentsSet::New(pipeline, 720, 720)
            .COLOR_ATTACHMENT<0>(color_attachment)
            .DEPTH_STENCIL_ATTACHMENT(depth_attachment);


        auto entitys = scene.buildEntitys();

        descriptors_sets.resize(entitys.size());
        for (int i = 0; i < entitys.size(); i++)
        {
            descriptors_sets[i] = RHIDescriptorsSet::New(pipeline)
                .DESCRIPTOR<0, 0>(camera_uniform)
                .DESCRIPTOR<0, 1>(light_uniform)
                .DESCRIPTOR<0, 2>(camera_pos_uniform)
                .DESCRIPTOR<1, 0>(entitys[i].material->textureShaderResource);
        }

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

        struct Data
        {
            glm::mat4x4 view;
            glm::mat4x4 project;
        }data;

        data.view = activeCamera.getViewMatrix();
        data.project = activeCamera.getProjectionMatrix();
        camera_uniform->update(&data, sizeof(data), 0);
        camera_pos_uniform->update(&activeCamera.position, sizeof(activeCamera.position), 0);

        struct Light
        {
            alignas(16) glm::vec3 pos;
            alignas(16) glm::vec3 color;
        };
        Light light;
        light.pos = glm::vec3(-3, 15, -8);
        light.color = glm::vec3(1, 1, 1);
        light_uniform->update(&light, sizeof(light), 0);

        IRHI::Get()->beginFrame();
        IRHI::Get()->resourceBarrier(image, RHIResourceState::render_target, RHIResourceState::transfer_dst);
        IRHI::Get()->resourceBarrier(depthImage, RHIResourceState::depth_stencil_write, RHIResourceState::transfer_dst);
        IRHI::Get()->clearImageResource(image);
        IRHI::Get()->clearImageResource(depthImage);
        IRHI::Get()->resourceBarrier(image, RHIResourceState::transfer_dst, RHIResourceState::render_target);
        IRHI::Get()->resourceBarrier(depthImage, RHIResourceState::transfer_dst, RHIResourceState::depth_stencil_write);

        RHIExecutor executor(pipeline, attachementSet);
        executor.beginPass();
        auto entitys = scene.buildEntitys();
        for (int i = 0; i < entitys.size(); i++)
        {
            executor.bindDescriptorSet(descriptors_sets[i].get());
            executor.bindVertexBuffer(entitys[i].mesh->vertex_buffer.get());
            executor.bindIndexBuffer(entitys[i].mesh->index_buffer.get());
            executor.drawIndex(entitys[i].mesh->subMeshes[entitys[i].subMeshId].size, 1, entitys[i].mesh->subMeshes[entitys[i].subMeshId].offset, 0, 0);
        }
        executor.endPass();

        IRHI::Get()->endFrame(image);

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