#include "Engine/Engine/Public/Engine.h"
#include "Engine/Render/Private/Scene.h"
#include "Engine/File/Public/yjw_file_system_module_header.h"
#include "Engine/Render/Public/yjw_render_system.h"
#include "Engine/Asset/Public/Asset.h"
#include "Engine/Engine/Private/Editor/yjw_editor_ui.h"
#include <chrono>
#include <ctime>
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/Engine/Public/Framework/Level.h"

#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_layer.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"
#include "Engine/Engine/Public/Framework/Render/IRenderModule.h"

namespace yjw
{
    Engine::Engine()
    {
        m_world = new World();
        m_ui = new EditorUI(m_world);
    }

    Engine::~Engine()
    {
        
    }

    void Engine::run()
    {
        initialize();
        while (!shouldShutdown) {
            mainLoop();
        }
        cleanup();
    }

    void Engine::shutdown()
    {
        GetModule<IRenderModule>()->Shutdown();
        shouldShutdown = true;
    }

    void Engine::initialize()
    {
        GetModule<IRenderModule>()->Startup();
        GetModule<IRenderModule>()->AttachScene(m_world->GetScene());
        GetModule<IRenderModule>()->AttachUI(m_ui);
        //m_world->GetLevel()->SpawnActor<TestBoxActor>("test box");
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

        GetModule<IRenderModule>()->Tick(deltaTime);
        
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