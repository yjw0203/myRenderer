#pragma once

#include "yjw_render_system.h"
#include "../rhi/yjw_rhi.h"
#include "../WindowManager/yjw_windows_manager.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace yjw
{

    void RenderSystem::initialize()
    {
        WindowsManager::get().initialize();
        rhi::CreateInfo rhi_createinfo{};
        rhi_createinfo.window = WindowsManager::get().window;
        rhi::rhiInit(rhi_createinfo);
    }
    rhi::DrawItem drawItem;
    void RenderSystem::tick()
    {
        WindowsManager::get().loop();
        rhi::rhiBeginFrame();
        drawItem.draw();
        rhi::rhiEndFrame();
    }
    void RenderSystem::shutdown()
    {
        WindowsManager::get().shutdown();

    }


}