#pragma once

#include "../yjw_startup_helper.h"
#include "../yjw_global_delegate.h"

class GLFWwindow;

namespace yjw
{

    class WindowsManager
    {
    public:
        static WindowsManager &get()
        {
            static WindowsManager instance;
            return instance;
        }
        void startup()
        {
            CoreDelegate::OnApplicationInitialize.Register([this] {this->initialize(); });
            CoreDelegate::OnApplicationLoop.Register([this] {this->loop(); });
            CoreDelegate::OnApplicationShutdown.Register([this] {this->shutdown(); });
        }
        static SimpleMulticastDelegate OnInitializeComplete;

    private:
        void initialize();
        void loop();
        void shutdown();
        GLFWwindow* window = nullptr;
    };

    HelpStartup(WindowsManager)
}