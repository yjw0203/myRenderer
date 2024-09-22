#pragma once


namespace yjw
{
    class RenderSystem;
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
    };

}