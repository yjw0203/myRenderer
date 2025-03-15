#pragma once

namespace rhi
{
    class ImGuiUI;
}

namespace yjw
{
    class IRenderModule;
    class World;
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

        World* m_world{};
        class ::rhi::ImGuiUI* m_ui;

        IRenderModule* m_render_module = nullptr;
    };

}