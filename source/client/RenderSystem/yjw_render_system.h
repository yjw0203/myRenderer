#pragma once

#include "client/yjw_global_delegate.h"
#include "client/yjw_module_interface.h"

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
    };

    REGISTER_DELEGATE(OnApplicationInitializedDelegate, []() {RenderSystem::get().initialize(); })
    REGISTER_DELEGATE(OnApplicationLoopDelegate, []() {RenderSystem::get().tick(); })
    REGISTER_DELEGATE(OnApplicationShutdownDelegate, []() {RenderSystem::get().shutdown(); })

}