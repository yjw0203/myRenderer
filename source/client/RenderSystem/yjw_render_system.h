#pragma once

#include "client/yjw_global_delegate.h"
#include "client/yjw_module_interface.h"
#include "yjw_render_camera.h"
#include "yjw_scene.h"

namespace yjw
{
    class RenderSystem : IModule
    {
    public:
        
        static RenderSystem& get()
        {
            static RenderSystem instance;
            return instance;
        }

        virtual void startup() override
        {

        }

        void initialize();
        void tick();
        void shutdown();

        float deltaTime = 0.f;

        class RenderCamera activeCamera;

        Scene scene;
    };

}