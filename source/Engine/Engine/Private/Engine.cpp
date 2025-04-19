#include "Engine/Engine/Public/Engine.h"
#include "Engine/Render/Private/Scene.h"
#include "Engine/File/Public/yjw_file_system_module_header.h"
#include "Engine/Asset/Public/Asset.h"
#include <chrono>
#include <ctime>
#include "Engine/Engine/Public/Window.h"
#include "Engine/Engine/Private/Editor/MajorEditor.h"

namespace yjw
{
    Engine::Engine()
    {
        Window::Initialize();
        m_editor = new MajorEditor();
    }

    Engine::~Engine()
    {
        
    }

    void Engine::run()
    {
        initialize();
        while (!shouldShutdown) {
            mainLoop();
            Window::PoolEvents();
        }
        cleanup();
    }

    void Engine::shutdown()
    {
        rdDestroy();
        Window::Shutdown();
        shouldShutdown = true;
    }

    void Engine::initialize()
    {
        rdInit();

        m_editor->Startup();
    }
    void Engine::mainLoop()
    {
        static time_t currentRealTime = 0;

        std::chrono::system_clock::time_point time_point_now = std::chrono::system_clock::now();
        std::chrono::system_clock::duration duration_since_epoch = time_point_now.time_since_epoch();
        time_t microseconds_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(duration_since_epoch).count();
        time_t time = microseconds_since_epoch;
        time_t delta_time_micro = time - currentRealTime;
        if (currentRealTime == 0)
        {
            delta_time_micro = 0;
        }
        float deltaTime = delta_time_micro / 1000000.0f;
        currentRealTime = time;

        rdTick(deltaTime);
        m_editor->Tick();
        AssetManager::Get()->process();

    }
    void Engine::cleanup()
    {
        
    }

    void Engine::loadModules()
    {
        FileSystemModule::Get()->StartUp();
    }

}