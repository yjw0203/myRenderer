#pragma once


namespace yjw
{
    class Application
    {
    public:
        void run();
        void shutdown();

        static Application& get()
        {
            static Application instance;
            return instance;
        }

    private:
        void initialize();
        void mainLoop();
        void cleanup();
        void loadModules();
    private:
        bool shouldShutdown = false;
    };

}