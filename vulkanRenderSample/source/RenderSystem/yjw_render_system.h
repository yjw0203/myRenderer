#pragma once

#include "../yjw_startup_helper.h"
#include "../yjw_global_delegate.h"


namespace yjw
{
    class RenderSystem
    {
    public:
        static RenderSystem& get()
        {
            static RenderSystem instance;
            return instance;
        }
        void startup()
        {
            CoreDelegate::OnApplicationInitialize.Register([this] {this->initialize(); });
            CoreDelegate::OnApplicationLoop.Register([this] {this->tick(); });
            CoreDelegate::OnApplicationShutdown.Register([this] {this->shutdown(); });
        }

    private:
        void initialize();
        void tick();
        void shutdown();
    };

    HelpStartup(RenderSystem)
}