#pragma once

#include <map>
#include <functional>
#include "Engine/RHI/Public/rpi/yjw_rpi.h"
struct GLFWwindow;

namespace yjw
{
    class Window
    {
    public:
        Window();
        ~Window();
        static void Initialize();
        static void PoolEvents();
        static void Shutdown();
        rpi::RPIWindow GetRPIWindow();
        void Present(rpi::RPIContext context);
    private:
        GLFWwindow* m_glfw_window = nullptr;
        rpi::RPIWindow m_rpi_window;
    };

    class View
    {
    public:

    };

    class InputDispatcher
    {
    public:
        static void Register();
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
        static void mouseScollCallback(GLFWwindow* window, double xoffset, double yoffset);

        static void registerKeyEvent(int key, int action, int mods, std::function<void(void)> func);
        static void registerMouseButtonEvent(int button, int action, int mods, std::function<void(void)> func);
        static void registerMousePosEvent(std::function<void(float, float)> func);
        static void registerMouseScrollEvent(std::function<void(float, float)> func);

    private:
        static std::map< long long, std::vector<std::function<void(void)>> > keyFunctions;
        static std::map< long long, std::vector<std::function<void(void)>> > mouseButtonFunctions;
        static std::vector<std::function<void(float, float)>> mousePosFunctions;
        static std::vector<std::function<void(float, float)>> mouseScrollFunctions;
    };
}