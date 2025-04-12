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

        class Window* m_window = nullptr;
        class ViewProxy* m_view = nullptr;

        class MajorEditor* m_editor = nullptr;
    };

}