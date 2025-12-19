#pragma once
#include "Engine/Utils/Public/yjw_delegate.h"

EXTERN_DELEGATE(EngineStartup);

class Engine
{
public:
    Engine();
    ~Engine();
    void run();
    void shutdown();
    void mainLoop();

private:
    void initialize();
    void cleanup();
    void loadModules();
private:
    bool shouldShutdown = false;

    class MajorEditor* m_editor = nullptr;
};
