#pragma once
#include "Engine/Render/Private/Scene.h"
#include "Engine/Engine/Public/Module/IModule.h"
namespace yjw
{
    class IRenderModule : public IModule
    {
    public:
        virtual void AttachScene(Scene* scene) = 0;
    };

    template<>
    IRenderModule* CreateModule();
}