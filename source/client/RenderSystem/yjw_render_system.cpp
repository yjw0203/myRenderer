#pragma once

#include "yjw_render_system.h"
#include "rhi/rhi/yjw_rhi_header.h"
#include "client/WindowManager/yjw_windows_manager.h"
#include "generate/projectInfo.h"
#include "glm/glm.hpp"
#include "yjw_render_camera.h"

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

    RHITexture2DFromFile* srv_image = nullptr;
    RHIResourceView* srv_imageView = nullptr;

    RHIBuffer* vertex_buffer = nullptr;
    RHIBuffer* index_buffer = nullptr;

    struct Vex
    {
        glm::vec3 pos;
        glm::vec2 uv;
    };
    Vex vex[4] = {
        {glm::vec3(-0.5,-0.5,0.5),glm::vec2{0,0}},
        {glm::vec3(-0.5,0.5,0.5),glm::vec2{0,1}},
        {glm::vec3(0.5,-0.5,0.5),glm::vec2{1,0}},
        {glm::vec3(0.5,0.5,0.5),glm::vec2{1,1}}
    };

    uint32_t index[6] = { 0,1,2,1,3,2 };


    void RenderSystem::initialize()
    {
        WindowsManager::get().initialize();
        CreateInfo rhi_createinfo{};
        rhi_createinfo.window = WindowsManager::get().window;
        IRHI::Get()->initialize(rhi_createinfo);

        RenderCamera camera;
        camera.position = glm::vec3(0.6, 0.6, 0.6);
        camera.direction = glm::vec3(0, 0, 0) - camera.position;

        glm::mat4x4 vp = camera.getViewProjectionMatrix();
        for (int i = 0; i < 4; i++)
        {
            glm::vec4 x = (vp * glm::vec4(vex[i].pos, 1.0f));
            vex[i].pos = glm::vec3(x.x, x.y, x.z);
        }

        vs = new RHIShader(SHADER_FILE(test_vert.spv));
        ps = new RHIShader(SHADER_FILE(test_frag.spv));

        vs_view = new RHIShaderView(vs, RHIShaderType::vertex_shader, "main");
        ps_view = new RHIShaderView(ps, RHIShaderType::pixel_shader, "main");
         
        image = new RHITexture2D(720, 720, 1, RHIFormat::R8G8B8A8_unorm, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        imageView = new RHIResourceView(ResourceViewType::rtv, image, RHIFormat::R8G8B8A8_unorm);

        srv_image = new RHITexture2DFromFile(RESOURCE_FILE(sjy.png));
        srv_imageView = new RHIResourceView(ResourceViewType::srv, srv_image, RHIFormat::R8G8B8A8_srgb);

        vertex_buffer = new RHIBuffer(sizeof(vex), RHIResourceUsageBits::allow_vertex_buffer, RHIMemoryType::default_);
        index_buffer = new RHIBuffer(sizeof(index), RHIResourceUsageBits::allow_index_buffer, RHIMemoryType::default_);

        IRHI::Get()->writeResourceImmidiately(vertex_buffer, vex, sizeof(vex));
        IRHI::Get()->writeResourceImmidiately(index_buffer, index, sizeof(index));

        ps_view->setDataTexture("myTexture", srv_imageView);

        draw_template = (new DefaultDrawTemplate())
            ->setRasterizationState(Rasterization_default)
            ->setColorBlendState(ColorBlend_default)
            ->setVertexShaderView(vs_view)
            ->setPixelShaderView(ps_view)
            ->setRenderTarget(1, imageView, nullptr)
            ->setVertexBuffer(vertex_buffer, VertexLayout().push(RHIFormat::R32G32B32_sfloat).push(RHIFormat::R32G32_sfloat))
            ->setIndexBuffer(index_buffer)
            ->setDrawIndex(6, 1, 0, 0, 0);

        draw_template->build();
    }
    
    void RenderSystem::tick()
    {
        IRHI::Get()->beginFrame();
        draw_template->draw();
        IRHI::Get()->endFrame(image);
    }
    void RenderSystem::shutdown()
    {
        WindowsManager::get().shutdown();

    }


}