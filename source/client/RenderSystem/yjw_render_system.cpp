#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "yjw_render_system.h"
#include "rhi/rhi/yjw_rhi_header.h"
#include "client/WindowManager/yjw_windows_manager.h"
#include "generate/projectInfo.h"

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

    RHITexture2D* srv_image = nullptr;
    RHIResourceView* srv_imageView = nullptr;
    void RenderSystem::initialize()
    {
        WindowsManager::get().initialize();
        CreateInfo rhi_createinfo{};
        rhi_createinfo.window = WindowsManager::get().window;
        IRHI::Get()->initialize(rhi_createinfo);

        vs = new RHIShader(SHADER_FILE(test_vert.spv));
        ps = new RHIShader(SHADER_FILE(test_frag.spv));

        vs_view = new RHIShaderView(vs, RHIShaderType::vertex_shader, "main");
        ps_view = new RHIShaderView(ps, RHIShaderType::pixel_shader, "main");

        image = new RHITexture2D(720, 720, 1, RHIFormat::R8G8B8A8_unorm, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        imageView = new RHIResourceView(ResourceViewType::rtv, image, RHIFormat::R8G8B8A8_unorm);

        srv_image = new RHITexture2D(720, 720, 1, RHIFormat::R8G8B8A8_unorm, RHIResourceUsageBits::none, RHIMemoryType::default_);
        srv_imageView = new RHIResourceView(ResourceViewType::srv, srv_image, RHIFormat::R8G8B8A8_unorm);

        IRHI::Get()->resourceBarrierImmidiately(srv_imageView, RHIResourceState::undefine, RHIResourceState::shader_resource_read);

        ps_view->setDataTexture("myTexture", srv_imageView);

        draw_template = (new DefaultDrawTemplate())
            ->setRasterizationState(Rasterization_default)
            ->setColorBlendState(ColorBlend_default)
            ->setVertexShaderView(vs_view)
            ->setPixelShaderView(ps_view)
            ->setRenderTarget(1, imageView, nullptr);
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