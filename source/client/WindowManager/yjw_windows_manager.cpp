#include "yjw_windows_manager.h"
#include "client/yjw_application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


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

}