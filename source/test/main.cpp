#include "iostream"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "RHI/rhi/yjw_rhi_header.h"

using namespace rhi;

GLFWwindow* window = nullptr;
RHIInstance* instance = nullptr;
RHIDevice* device = nullptr;
RHISwapChain* swapchain = nullptr;

RHIRenderPass* renderPass = nullptr;
RHIRenderPipeline* renderPipeline = nullptr;
RHIContext* context = nullptr;

void initRender()
{
    RHIInstanceConfig config{};
    config.rhiType = ERHIType::vulkan;
    config.isDebugMode = true;
    instance = new RHIInstance(config);
    device = instance->CreateDevice();
    swapchain = device->CreateSwapchain(window);
    context = device->CreateContext();

    RHIRenderPassDescriptor renderpassDesc{};
    renderpassDesc.colorAttachmentCount = 1;
    renderpassDesc.colorAttachments[0] = swapchain->GetBackTextureView()
        ;
    renderPass = device->CreateRenderPass(renderpassDesc);

    RHIShader* vs = device->CreateShaderByBinaryUrl("D:/workspace/projects/vulkanRenderSample/shaders/test1_vert.spv");
    RHIShader* ps = device->CreateShaderByBinaryUrl("D:/workspace/projects/vulkanRenderSample/shaders/test1_frag.spv");

    RHIRenderPipelineDescriptor pipelineDesc{};
    pipelineDesc.vs = vs;
    pipelineDesc.vs_entry = "main";
    pipelineDesc.ps = ps;
    pipelineDesc.ps_entry = "main";
    renderPipeline = device->CreateRenderPipeline(pipelineDesc);

    vs->release();
    ps->release();
}

void render()
{
    context->BeginPass(renderPass);
    context->SetRenderPipeline(renderPipeline);
    context->Draw(3, 1, 0, 0);
    context->EndPass();
    context->Submit();
    context->Present(swapchain, false);
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