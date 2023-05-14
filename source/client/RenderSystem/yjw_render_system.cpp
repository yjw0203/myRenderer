#pragma once

#include "yjw_render_system.h"
#include "rhi/rhi/yjw_rhi_header.h"
#include "client/WindowManager/yjw_windows_manager.h"
#include "generate/projectInfo.h"
#include "glm/glm.hpp"
#include "yjw_render_camera.h"
#include "yjw_mesh.h"
#include <chrono>
#include <ctime>

namespace yjw
{
    using namespace rhi;
    std::vector<DefaultDrawTemplate*> draw_templates;
    std::vector<RHIShaderView*> vs_views;
    std::vector<RHIShaderView*> ps_views;
    RHIShader* vs = nullptr;
    RHIShader* ps = nullptr;
    RHITexture2D* image = nullptr;
    RHIResourceView* imageView = nullptr;

    RHITexture2D* depthImage = nullptr;
    RHIResourceView* depthImageView = nullptr;

    RHITexture2DFromFile* srv_image = nullptr;
    RHIResourceView* srv_imageView = nullptr;

    RHIBuffer* vertex_buffer = nullptr;
    RHIBuffer* index_buffer = nullptr;
    RHIBuffer* camera_uniform_buffer = nullptr;
    RHIResourceView* camera_uniform_buffer_view = nullptr;
    RHIBuffer* camera_uniform_pos_buffer = nullptr;
    RHIResourceView* camera_uniform_pos_buffer_view = nullptr;
    RHIBuffer* light_uniform_buffer = nullptr;
    RHIResourceView* light_uniform_buffer_view = nullptr;

    Mesh naxita_mesh;
    Mesh heita_mesh;
    Mesh hutao_mesh;

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


    void RenderSystem::initialize()
    {
        WindowsManager::get().initialize();
        CreateInfo rhi_createinfo{};
        rhi_createinfo.window = WindowsManager::get().window;
        IRHI::Get()->initialize(rhi_createinfo);

        naxita_mesh = *Mesh::load(RESOURCE_FILE(cao),"纳西妲.pmx");
        heita_mesh = *Mesh::load(RESOURCE_FILE(heita),"黑塔.pmx");
        hutao_mesh = *Mesh::load(RESOURCE_FILE(hutao),"胡桃.pmx");
        activeCamera.position = glm::vec3(9, -10, 0);
        //activeCamera.direction = glm::vec3(0, 0, 0) - activeCamera.position;
        activeCamera.direction = glm::vec3(0, 0, 1);
        activeCamera.up = glm::vec3(0, -1, 0);

        vs = new RHIShader(SHADER_FILE(test_vert.spv));
        ps = new RHIShader(SHADER_FILE(test_frag.spv));

        image = new RHITexture2D(720, 720, 1, RHIFormat::R8G8B8A8_unorm, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        imageView = new RHIResourceView(ResourceViewType::rtv, image, RHIFormat::R8G8B8A8_unorm);
        IRHI::Get()->resourceBarrierImmidiately(image, RHIResourceState::undefine, RHIResourceState::render_target);

        depthImage = new RHITexture2D(720, 720, 1, RHIFormat::D24_unorm_S8_uint, RHIResourceUsageBits::allow_depth_stencil, RHIMemoryType::default_);
        depthImageView = new RHIResourceView(ResourceViewType::dsv, depthImage, RHIFormat::D24_unorm_S8_uint);
        IRHI::Get()->resourceBarrierImmidiately(depthImage, RHIResourceState::undefine, RHIResourceState::depth_stencil_write);

        Mesh& mesh = heita_mesh;
        vertex_buffer = new RHIBuffer(sizeof(MeshVertex)*mesh.vertices.size(), RHIResourceUsageBits::allow_vertex_buffer, RHIMemoryType::default_);
        index_buffer = new RHIBuffer(sizeof(int)*mesh.indices.size(), RHIResourceUsageBits::allow_index_buffer, RHIMemoryType::default_);
        camera_uniform_buffer = new RHIBuffer(128 , RHIResourceUsageBits::none, RHIMemoryType::upload);
        camera_uniform_buffer_view = new RHIResourceView(ResourceViewType::buffer, camera_uniform_buffer, RHIFormat::unknow);
        camera_uniform_pos_buffer = new RHIBuffer(64, RHIResourceUsageBits::none, RHIMemoryType::upload);
        camera_uniform_pos_buffer_view = new RHIResourceView(ResourceViewType::buffer, camera_uniform_pos_buffer, RHIFormat::unknow);
        light_uniform_buffer = new RHIBuffer(128, RHIResourceUsageBits::none, RHIMemoryType::upload);
        light_uniform_buffer_view = new RHIResourceView(ResourceViewType::buffer, light_uniform_buffer, RHIFormat::unknow);

        IRHI::Get()->writeResourceImmidiately(vertex_buffer,  mesh.vertices.data(), sizeof(MeshVertex) * mesh.vertices.size());
        IRHI::Get()->writeResourceImmidiately(index_buffer, mesh.indices.data(), sizeof(int) * mesh.indices.size());

        //vs_view->setDataBuffer("camera", camera_uniform_buffer_view);
        //ps_view->setDataTexture("myTexture", srv_imageView);

        draw_templates.resize(mesh.materials.size());
        vs_views.resize(mesh.materials.size());
        ps_views.resize(mesh.materials.size());
        for (int i = 0; i < draw_templates.size(); i++)
        {
            vs_views[i] = new RHIShaderView(vs, RHIShaderType::vertex_shader, "main");;
            ps_views[i] = new RHIShaderView(ps, RHIShaderType::pixel_shader, "main");
            vs_views[i]->setDataBuffer("camera", camera_uniform_buffer_view);
            ps_views[i]->setDataTexture("myTexture", mesh.materials[i].textureView);
            ps_views[i]->setDataBuffer("camerapos", camera_uniform_pos_buffer_view);
            ps_views[i]->setDataBuffer("light", light_uniform_buffer_view);

            draw_templates[i] = (new DefaultDrawTemplate())
                ->setRasterizationState(Rasterization_default)
                ->setColorBlendState(ColorBlend_default)
                ->setDepthStencilState(DepthStencil_default)
                ->setVertexShaderView(vs_views[i])
                ->setPixelShaderView(ps_views[i])
                ->setRenderTarget(1, imageView, depthImageView)
                ->setVertexBuffer(vertex_buffer, VertexLayout().push(RHIFormat::R32G32B32_sfloat).push(RHIFormat::R32G32B32_sfloat).push(RHIFormat::R32G32_sfloat))
                ->setIndexBuffer(index_buffer)
                ->setDrawIndex(mesh.materials[i].size, 1, mesh.materials[i].offset, 0, 0);
            draw_templates[i]->build();
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
        IRHI::Get()->writeResourceImmidiately(camera_uniform_buffer, &data, sizeof(data));
        IRHI::Get()->writeResourceImmidiately(camera_uniform_pos_buffer, &activeCamera.position, sizeof(activeCamera.position));

        struct Light
        {
            alignas(16) glm::vec3 pos;
            alignas(16) glm::vec3 color;
        };
        Light light;
        light.pos = glm::vec3(-3, 15, -8);
        light.color = glm::vec3(1, 1, 1);
        IRHI::Get()->writeResourceImmidiately(light_uniform_buffer, &light, sizeof(light));

        IRHI::Get()->beginFrame();
        IRHI::Get()->resourceBarrier(image, RHIResourceState::render_target, RHIResourceState::transfer_dst);
        IRHI::Get()->resourceBarrier(depthImage, RHIResourceState::depth_stencil_write, RHIResourceState::transfer_dst);
        IRHI::Get()->clearImageResource(image);
        IRHI::Get()->clearImageResource(depthImage);
        IRHI::Get()->resourceBarrier(image, RHIResourceState::transfer_dst, RHIResourceState::render_target);
        IRHI::Get()->resourceBarrier(depthImage, RHIResourceState::transfer_dst, RHIResourceState::depth_stencil_write);

        for (auto draw_template : draw_templates)
        {
            draw_template->draw();
        }
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