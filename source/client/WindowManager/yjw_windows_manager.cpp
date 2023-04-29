#include "yjw_windows_manager.h"
#include "client/yjw_application.h"
#include "client/RenderSystem/yjw_render_camera.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

const int WIDTH = 720;
const int HEIGHT = 720;

namespace yjw
{
    void WindowsManager::initialize()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "WOW!", nullptr, nullptr);
        glfwSetKeyCallback(window, InputDispatcher::keyCallback);

        InputDispatcher::Register();
    }
    void WindowsManager::loop()
    {
        if (glfwWindowShouldClose(window))
        {
            Application::get().shutdown();
        }
        else
        {
            glfwPollEvents();
        }
    }
    void WindowsManager::shutdown()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    std::map< long long, std::vector<std::function<void(void)>> > InputDispatcher::keyFunctions = std::map< long long, std::vector<std::function<void(void)>> >();

    void InputDispatcher::Register()
    {
        RenderCameraInputDispatcher::Register();
    }

    void InputDispatcher::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        long long hash_key = (key << 20ll) + (action << 10ll) + mods;
        std::vector < std::function<void(void)> >& funcs = keyFunctions[hash_key];
        for (std::function<void(void)>& func : funcs)
        {
            func();
        }
    }

    void InputDispatcher::registerKeyEvent(int key, int action, int mods, std::function<void(void)> func)
    {
        long long hash_key = (key << 20ll) + (action << 10ll) + mods;
        keyFunctions[hash_key].push_back(func);
    }

}