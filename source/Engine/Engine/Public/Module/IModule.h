#pragma once
#include "Engine/Engine/Public/Render/Scene.h"

namespace yjw
{
    class IModule
    {
    public:
        virtual void Startup() = 0;
        virtual void Shutdown() = 0;
        virtual void Tick(float deltaTime) = 0;
    };

    template<typename T>
    T* CreateModule();
}