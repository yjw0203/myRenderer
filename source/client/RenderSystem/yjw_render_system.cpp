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
#include "pass/yjw_pass_gbuffer.h"
#include "pass/yjw_pass_deferred_shading.h"

namespace yjw
{
    using namespace rhi;

    std::shared_ptr<GBufferPass> gbuffer_pass;
    std::shared_ptr<DeferredShadingPass> deferred_shading_pass;

    RHITexture2D* albedo_image = nullptr;
    RHITexture2D* normal_image = nullptr;
    RHITexture2D* depthImage = nullptr;
    RHITexture2D* colorImage = nullptr;
    std::shared_ptr<RHIColorAttachment> albedo_attachment;
    std::shared_ptr<RHIColorAttachment> normal_attachment;
    std::shared_ptr<RHIColorAttachment> color_attachment;
    std::shared_ptr<RHIDepthStencilAttachment> depth_attachment;

    std::shared_ptr<RHIShaderResourceTexture> albedo_shader_resource;
    std::shared_ptr<RHIShaderResourceTexture> normal_shader_resource;
    std::shared_ptr<RHIShaderResourceTexture> depth_shader_resource;

    std::shared_ptr<Model> naxita;
    std::shared_ptr<Model> heita;
    std::shared_ptr<Model> hutao;

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
        //scene.models.push_back(heita);
        //scene.models.push_back(hutao);

        activeCamera.position = glm::vec3(1, 12, -16);
        //activeCamera.direction = glm::vec3(0, 0, 0) - activeCamera.position;
        activeCamera.direction = glm::vec3(0, -0.2,1);
        activeCamera.up = glm::vec3(0, -1, -0.2);

        albedo_image = new RHITexture2D(720, 720, 1, RHIFormat::R8G8B8A8_snorm, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        IRHI::Get()->resourceBarrierImmidiately(albedo_image, RHIResourceState::undefine, RHIResourceState::render_target);

        normal_image = new RHITexture2D(720, 720, 1, RHIFormat::R8G8B8A8_snorm, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        IRHI::Get()->resourceBarrierImmidiately(normal_image, RHIResourceState::undefine, RHIResourceState::render_target);
        
        depthImage = new RHITexture2D(720, 720, 1, RHIFormat::D24_unorm_S8_uint, RHIResourceUsageBits::allow_depth_stencil, RHIMemoryType::default_);
        IRHI::Get()->resourceBarrierImmidiately(depthImage, RHIResourceState::undefine, RHIResourceState::depth_stencil_write);

        colorImage = new RHITexture2D(720, 720, 1, RHIFormat::R8G8B8A8_unorm, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        IRHI::Get()->resourceBarrierImmidiately(colorImage, RHIResourceState::undefine, RHIResourceState::render_target);

        albedo_attachment = std::make_shared<RHIColorAttachment>(albedo_image, RHIFormat::R8G8B8A8_snorm);
        normal_attachment = std::make_shared<RHIColorAttachment>(normal_image, RHIFormat::R8G8B8A8_snorm);
        depth_attachment = std::make_shared<RHIDepthStencilAttachment>(depthImage, RHIFormat::D24_unorm_S8_uint);
        color_attachment = std::make_shared<RHIColorAttachment>(colorImage, RHIFormat::R8G8B8A8_unorm);

        albedo_shader_resource = std::make_shared<RHIShaderResourceTexture>(albedo_image, RHIFormat::R8G8B8A8_snorm);
        normal_shader_resource = std::make_shared<RHIShaderResourceTexture>(normal_image, RHIFormat::R8G8B8A8_snorm);
        depth_shader_resource = std::make_shared<RHIShaderResourceTexture>(depthImage, RHIFormat::D24_unorm_S8_uint);


        gbuffer_pass = std::make_shared<GBufferPass>();
        deferred_shading_pass = std::make_shared<DeferredShadingPass>();
        deferred_shading_pass->buildPSO();
        gbuffer_pass->buildPSO();

        gbuffer_pass->registerTexture(albedo_attachment, normal_attachment, depth_attachment);
        deferred_shading_pass->registerTexture(albedo_shader_resource, normal_shader_resource, depth_shader_resource, color_attachment);

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

        gbuffer_pass->setupData();
        deferred_shading_pass->setupData();

        IRHI::Get()->beginFrame();
        IRHI::Get()->resourceBarrier(albedo_image, RHIResourceState::render_target, RHIResourceState::transfer_dst);
        IRHI::Get()->resourceBarrier(normal_image, RHIResourceState::render_target, RHIResourceState::transfer_dst);
        IRHI::Get()->resourceBarrier(depthImage, RHIResourceState::depth_stencil_write, RHIResourceState::transfer_dst);
        IRHI::Get()->resourceBarrier(colorImage, RHIResourceState::render_target, RHIResourceState::transfer_dst);
        IRHI::Get()->clearImageResource(albedo_image);
        IRHI::Get()->clearImageResource(normal_image);
        IRHI::Get()->clearImageResource(depthImage);
        IRHI::Get()->clearImageResource(colorImage);
        IRHI::Get()->resourceBarrier(albedo_image, RHIResourceState::transfer_dst, RHIResourceState::render_target);
        IRHI::Get()->resourceBarrier(normal_image, RHIResourceState::transfer_dst, RHIResourceState::render_target);
        IRHI::Get()->resourceBarrier(depthImage, RHIResourceState::transfer_dst, RHIResourceState::depth_stencil_write);
        IRHI::Get()->resourceBarrier(colorImage, RHIResourceState::transfer_dst, RHIResourceState::render_target);

        gbuffer_pass->recordCommand();
        IRHI::Get()->resourceBarrier(depthImage, RHIResourceState::depth_stencil_write, RHIResourceState::shader_resource_read);
        IRHI::Get()->resourceBarrier(albedo_image, RHIResourceState::render_target, RHIResourceState::shader_resource_read);
        IRHI::Get()->resourceBarrier(normal_image, RHIResourceState::render_target, RHIResourceState::shader_resource_read);
        deferred_shading_pass->recordCommand();
        IRHI::Get()->resourceBarrier(depthImage, RHIResourceState::shader_resource_read, RHIResourceState::depth_stencil_write);
        IRHI::Get()->resourceBarrier(albedo_image, RHIResourceState::shader_resource_read, RHIResourceState::render_target);
        IRHI::Get()->resourceBarrier(normal_image, RHIResourceState::shader_resource_read, RHIResourceState::render_target);

        IRHI::Get()->endFrame(colorImage);

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