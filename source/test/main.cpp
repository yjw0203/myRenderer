#include "iostream"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "RHI/rhi/yjw_rhi_instance.h"

using namespace rhi;

GLFWwindow* window = nullptr;
RHIInstance* instance = nullptr;
RHIDevice* device = nullptr;
RHISwapChain* swapchain = nullptr;

void initRender()
{
    RHIInstanceConfig config{};
    config.rhiType = ERHIType::vulkan;
    config.isDebugMode = true;
    instance = new RHIInstance(config);
    device = instance->CreateDevice();
    swapchain = device->CreateSwapchain(window);

    device->CreateShaderByBinaryUrl("D:/workspace/projects/vulkanRenderSample/shaders/gbuffer_vert.spv");
}

void render()
{
    swapchain->Prensent(false);
}


int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(800, 800, "WOW!", nullptr, nullptr);

    initRender();

    bool shouldClose = false;

    while (!shouldClose)
    {
        if (glfwWindowShouldClose(window))
        {
            shouldClose = true;
        }
        else
        {
            render();
            glfwPollEvents();
        }
    }
    return 0;
}