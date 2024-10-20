#include "Engine/Engine.h"
#include "Engine/Framework/Scene/Scene.h"
#include "File/yjw_file_system_module_header.h"
#include "Render/yjw_render_system.h"

namespace yjw
{
    Engine::Engine()
    {
        m_render_system = &RenderSystem::get();
    }

    Engine::~Engine()
    {
        
    }

    void Engine::run()
    {
        initialize();
        mainLoop();
        cleanup();
    }

    void Engine::shutdown()
    {
        shouldShutdown = true;
    }

    void Engine::initialize()
    {
        m_render_system->initialize();
    }
    void Engine::mainLoop()
    {
        while (!shouldShutdown) {
            m_render_system->tick();
        }
    }
    void Engine::cleanup()
    {
        m_render_system->shutdown();
    }

    void Engine::loadModules()
    {
        FileSystemModule::Get()->StartUp();
    }

}