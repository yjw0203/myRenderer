#include "Application/yjw_application.h"
#include "File/yjw_file_system_module_header.h"
#include "Render/yjw_render_system.h"

namespace yjw
{
    void Application::run()
    {
        initialize();
        mainLoop();
        cleanup();
    }

    void Application::shutdown()
    {
        shouldShutdown = true;
    }

    void Application::initialize()
    {
        RenderSystem::get().initialize();
    }
    void Application::mainLoop()
    {
        while (!shouldShutdown) {
            RenderSystem::get().tick();
        }
    }
    void Application::cleanup()
    {
        RenderSystem::get().shutdown();
    }

    void Application::loadModules()
    {
        FileSystemModule::Get()->StartUp();
    }

}