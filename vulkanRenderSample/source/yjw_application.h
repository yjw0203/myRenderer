#pragma once

#include "yjw_global_delegate.h"
#include "yjw_pre_register.h"

namespace yjw
{
    class Application
    {
    public:
        void run()
        {
            yjw::PreRegisterHelper::get().executePreRegister();
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
            CoreDelegate::OnApplicationInitialize.Broadcast();
        }
        void mainLoop()
        {
            while (!shouldShutdown) {
                CoreDelegate::OnApplicationLoop.Broadcast();
            }
        }
        void cleanup()
        {
            CoreDelegate::OnApplicationShutdown.Broadcast();
        }

        bool shouldShutdown = false;
    };

}