#pragma once

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
        GLFWwindow* window = nullptr;

        void initialize();
        void loop();
        void shutdown();
    };
}