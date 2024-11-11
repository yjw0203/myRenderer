#pragma once

namespace rhi
{
    class ImGuiUI;
}

namespace yjw
{
    class RenderSystem;
    class Scene;
    class Engine
    {
    public:
        Engine();
        ~Engine();
        void run();
        void shutdown();

    private:
        void initialize();
        void mainLoop();
        void cleanup();
        void loadModules();
    private:
        bool shouldShutdown = false;

        RenderSystem* m_render_system;
        Scene* m_scene;
        class ::rhi::ImGuiUI* m_ui;
    };

}