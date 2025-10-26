#pragma once
#include "Engine/Utils/Public/yjw_delegate.h"

EXTERN_DELEGATE(EngineStartup);

namespace yjw
{
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

        class MajorEditor* m_editor = nullptr;
    };

}