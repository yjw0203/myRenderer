#include "Engine/Engine/Public/Engine.h"
#include "Engine/Engine/Public/Framework/Scene/Scene.h"
#include "Engine/File/Public/yjw_file_system_module_header.h"
#include "Engine/Render/Public/yjw_render_system.h"
#include "Engine/Engine/Private/Editor/yjw_editor_ui.h"
#include <chrono>
#include <ctime>

#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_layer.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"

namespace yjw
{
    Engine::Engine()
    {
        m_render_system = new RenderSystem();
        m_scene = new Scene();
        m_ui = new EditorUI();
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
        shouldShutdown = true;
    }

    void Engine::initialize()
    {
        m_render_system->AttachScene(m_scene);
        m_render_system->AttachUI(m_ui);
        m_render_system->initialize();

        ((EditorUI*)m_ui)->m_scene_texture = dynamic_cast<rhi::RHIImguiLayer*>(rpi::RPIGetLayer(rhi::rhi_layer_imgui))->RegisterTexture("scene", m_render_system->GetSceneTexture().GetView());
        ((EditorUI*)m_ui)->m_scene = m_scene;
        m_scene->addBox();
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

        m_render_system->tick(deltaTime);

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