#pragma once
#include "Engine/Engine/Public/Render/Scene.h"
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