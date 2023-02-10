#pragma once

#include "yjw_global_delegate.h"
#include "yjw_module_interface.h"

namespace yjw
{
    class Application
    {
    public:
        void run()
        {
            initialize();
            mainLoop();
            cleanup();
        }
        void shutdown()
        {
            shouldShutdown = true;
        }

        static Application& get()
        {
            static Application instance;
            return instance;
        }

    private:
        void initialize()
        {
            BROADCAST_DELEGATE(OnApplicationInitializedDelegate)
        }
        void mainLoop()
        {
            while (!shouldShutdown) {
                BROADCAST_DELEGATE(OnApplicationLoopDelegate)
            }
        }
        void cleanup()
        {
            BROADCAST_DELEGATE(OnApplicationShutdownDelegate)
        }

        bool shouldShutdown = false;
    };

}