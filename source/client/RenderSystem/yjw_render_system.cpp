#pragma once

#include "yjw_render_system.h"
#include "rhi/rhi/yjw_rhi_header.h"
#include "client/WindowManager/yjw_windows_manager.h"
#include "generate/projectInfo.h"
#include "glm/glm.hpp"
#include "yjw_render_camera.h"
#include "yjw_mesh.h"

namespace yjw
{
    using namespace rhi;
    DefaultDrawTemplate* draw_template = nullptr;
    RHIShader* vs = nullptr;
    RHIShaderView* vs_view = nullptr;
    RHIShader* ps = nullptr;
    RHIShaderView* ps_view = nullptr;
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

    Mesh mesh;

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
        mesh = *Mesh::load(RESOURCE_FILE(ÐÇñ·ÌúµÀ-ºÚËþ/ºÚËþ.pmx));
        WindowsManager::get().initialize();
        CreateInfo rhi_createinfo{};
        rhi_createinfo.window = WindowsManager::get().window;
        IRHI::Get()->initialize(rhi_createinfo);

        activeCamera.position = glm::vec3(9, -10, 0);
        activeCamera.direction = glm::vec3(0, 0, 0) - activeCamera.position;
        activeCamera.up = glm::vec3(0, -1, 0);

        vs = new RHIShader(SHADER_FILE(test_vert.spv));
        ps = new RHIShader(SHADER_FILE(test_frag.spv));

        vs_view = new RHIShaderView(vs, RHIShaderType::vertex_shader, "main");
        ps_view = new RHIShaderView(ps, RHIShaderType::pixel_shader, "main");
         
        image = new RHITexture2D(720, 720, 1, RHIFormat::R8G8B8A8_unorm, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        imageView = new RHIResourceView(ResourceViewType::rtv, image, RHIFormat::R8G8B8A8_unorm);

        depthImage = new RHITexture2D(720, 720, 1, RHIFormat::D24_unorm_S8_uint, RHIResourceUsageBits::allow_depth_stencil, RHIMemoryType::default_);
        depthImageView = new RHIResourceView(ResourceViewType::dsv, depthImage, RHIFormat::D24_unorm_S8_uint);

        srv_image = new RHITexture2DFromFile(RESOURCE_FILE(ÐÇñ·ÌúµÀ-ºÚËþ/ÒÂ·þ.png));
        srv_imageView = new RHIResourceView(ResourceViewType::srv, srv_image, RHIFormat::R8G8B8A8_srgb);

        vertex_buffer = new RHIBuffer(sizeof(MeshVertex)*mesh.vertices.size(), RHIResourceUsageBits::allow_vertex_buffer, RHIMemoryType::default_);
        index_buffer = new RHIBuffer(sizeof(int)*mesh.indices.size(), RHIResourceUsageBits::allow_index_buffer, RHIMemoryType::default_);
        camera_uniform_buffer = new RHIBuffer(128 , RHIResourceUsageBits::none, RHIMemoryType::upload);
        camera_uniform_buffer_view = new RHIResourceView(ResourceViewType::buffer, camera_uniform_buffer, RHIFormat::unknow);

        IRHI::Get()->writeResourceImmidiately(vertex_buffer,  mesh.vertices.data(), sizeof(MeshVertex) * mesh.vertices.size());
        IRHI::Get()->writeResourceImmidiately(index_buffer, mesh.indices.data(), sizeof(int) * mesh.indices.size());

        vs_view->setDataBuffer("camera", camera_uniform_buffer_view);
        ps_view->setDataTexture("myTexture", srv_imageView);

        draw_template = (new DefaultDrawTemplate())
            ->setRasterizationState(Rasterization_default)
            ->setColorBlendState(ColorBlend_default)
            ->setDepthStencilState(DepthStencil_default)
            ->setVertexShaderView(vs_view)
            ->setPixelShaderView(ps_view)
            ->setRenderTarget(1, imageView, depthImageView)
            ->setVertexBuffer(vertex_buffer, VertexLayout().push(RHIFormat::R32G32B32_sfloat).push(RHIFormat::R32G32B32_sfloat).push(RHIFormat::R32G32_sfloat))
            ->setIndexBuffer(index_buffer)
            ->setDrawIndex(mesh.indices.size(), 1, 0, 0, 0);

        draw_template->build();


    }
    
    void RenderSystem::tick()
    {
        struct Data
        {
            glm::mat4x4 view;
            glm::mat4x4 project;
        }data;

        data.view = activeCamera.getViewMatrix();
        data.project = activeCamera.getProjectionMatrix();
        IRHI::Get()->writeResourceImmidiately(camera_uniform_buffer, &data, sizeof(data));

        IRHI::Get()->beginFrame();
        draw_template->draw();
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