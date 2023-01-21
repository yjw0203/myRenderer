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
    void RenderSystem::tick()
    {
        rhi::rhiBeginFrame();
        WindowsManager::get().loop();


        rhi::rhiEndFrame();
    }
    void RenderSystem::shutdown()
    {
        WindowsManager::get().shutdown();

    }


}