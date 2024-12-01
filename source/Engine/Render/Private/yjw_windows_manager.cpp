#include "yjw_windows_manager.h"
#include "Engine/Render/Private/yjw_render_camera.h"

//#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

const int WIDTH = 1200;
const int HEIGHT = 1200;

namespace yjw
{
    Window::Window()
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        m_glfw_window = glfwCreateWindow(WIDTH, HEIGHT, "WOW!", nullptr, nullptr);
        glfwSetKeyCallback(m_glfw_window, InputDispatcher::keyCallback);
        glfwSetMouseButtonCallback(m_glfw_window, InputDispatcher::mouseButtonCallback);
        glfwSetCursorPosCallback(m_glfw_window, InputDispatcher::mousePosCallback);
        glfwSetScrollCallback(m_glfw_window, InputDispatcher::mouseScollCallback);
        m_rpi_window = rpi::RPICreateWindow(m_glfw_window);
    }

    Window::~Window()
    {
        RPIDestroyWindow(m_rpi_window);
        glfwDestroyWindow(m_glfw_window);
    }

    void Window::Initialize()
    {
        glfwInit();
    }

    void Window::PoolEvents()
    {
        glfwPollEvents();
    }

    void Window::Shutdown()
    {
        glfwTerminate();
    }

    rpi::RPIWindow Window::GetRPIWindow()
    {
        return m_rpi_window;
    }

    void Window::Present(rpi::RPIContext context)
    {
        rpi::RPIPresent(context, m_rpi_window);
    }

    std::map< long long, std::vector<std::function<void(void)>> > InputDispatcher::keyFunctions = std::map< long long, std::vector<std::function<void(void)>> >();
    std::map< long long, std::vector<std::function<void(void)>> > InputDispatcher::mouseButtonFunctions = std::map< long long, std::vector<std::function<void(void)>> >();
    std::vector<std::function<void(float, float)>> InputDispatcher::mousePosFunctions = std::vector<std::function<void(float, float)>>();
    std::vector<std::function<void(float, float)>> InputDispatcher::mouseScrollFunctions = std::vector<std::function<void(float, float)>>();

    void InputDispatcher::Register()
    {
        
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

    void InputDispatcher::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        long long hash_key = (button << 20ll) + (action << 10ll) + mods;
        std::vector < std::function<void(void)> >& funcs = mouseButtonFunctions[hash_key];
        for (std::function<void(void)>& func : funcs)
        {
            func();
        }
    }

    void InputDispatcher::mousePosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        for (std::function<void(float,float)>& func : mousePosFunctions)
        {
            func(xpos, ypos);
        }
    }

    void InputDispatcher::mouseScollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        for (std::function<void(float, float)>& func : mouseScrollFunctions)
        {
            func(xoffset, yoffset);
        }
    }

    void InputDispatcher::registerKeyEvent(int key, int action, int mods, std::function<void(void)> func)
    {
        long long hash_key = (key << 20ll) + (action << 10ll) + mods;
        keyFunctions[hash_key].push_back(func);
    }

    void InputDispatcher::registerMouseButtonEvent(int button, int action, int mods, std::function<void(void)> func)
    {
        long long hash_key = (button << 20ll) + (action << 10ll) + mods;
        mouseButtonFunctions[hash_key].push_back(func);
    }

    void InputDispatcher::registerMousePosEvent(std::function<void(float, float)> func)
    {
        mousePosFunctions.push_back(func);
    }

    void InputDispatcher::registerMouseScrollEvent(std::function<void(float, float)> func)
    {
        mouseScrollFunctions.push_back(func);
    }

}