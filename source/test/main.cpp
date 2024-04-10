#include "iostream"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "RHI/rhi/yjw_rhi_header.h"

using namespace rhi;

RHIInstance* instance = nullptr;
RHIDevice* device = nullptr;

class Window
{
public:
    void create()
    {
        m_window = glfwCreateWindow(800, 800, "WOW!", nullptr, nullptr);
        m_swapchain = device->CreateSwapchain(m_window);
    }

    void destroy()
    {
        glfwDestroyWindow(m_window);
        m_swapchain->release();
    }
public:
    GLFWwindow* m_window = nullptr;
    RHISwapChain* m_swapchain = nullptr;
};

const  int countOfWindows = 2;
Window* windows[countOfWindows] = { nullptr,nullptr };

RHIRenderPass* renderPass = nullptr;
RHIRenderPipeline* renderPipeline = nullptr;
RHIResourceBinding* resourceBinding = nullptr;
RHIContext* context = nullptr;
RHITexture* rtTexture = nullptr;
RHITextureView* rtTextureView = nullptr;

RHITexture* srvTexture = nullptr;
RHITextureView* srvTextureView = nullptr;

RHITexture* createTexture(const char* filePath)
{
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(filePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    RHITextureDescriptor creation{};
    creation.resourceType = RHIResourceType::texture2D;
    creation.width = texWidth;
    creation.height = texHeight;
    creation.depthOrArraySize = 1;
    creation.miplevels = 1;
    creation.format = RHIFormat::R8G8B8A8_srgb;
    creation.usage = (int)RHIResourceUsageBits::allow_transfer_dst;
    creation.memoryType = RHIMemoryType::default_;
    RHITexture* texture = device->CreateTexture(creation);

    int imageSize = texWidth * texHeight * 4;
    texture->Update(pixels, imageSize);
    stbi_image_free(pixels);
    return texture;
}

void initRender()
{
    RHIInstanceConfig config{};
    config.rhiType = ERHIType::vulkan;
    config.isDebugMode = true;
    instance = new RHIInstance(config);
    device = instance->CreateDevice();
    
    context = device->CreateContext();

    RHIShader* vs1 = device->CreateShaderByBinaryUrl("D:/workspace/projects/vulkanRenderSample/shaders/gbuffer_vert.spv");
    RHIShader* vs = device->CreateShaderByBinaryUrl("D:/workspace/projects/vulkanRenderSample/shaders/test1_vert.spv");
    RHIShader* ps = device->CreateShaderByBinaryUrl("D:/workspace/projects/vulkanRenderSample/shaders/test1_frag.spv");

    RHIRenderPipelineDescriptor pipelineDesc{};
    pipelineDesc.vs = vs;
    pipelineDesc.vs_entry = "main";
    pipelineDesc.ps = ps;
    pipelineDesc.ps_entry = "main";
    renderPipeline = device->CreateRenderPipeline(pipelineDesc);

    resourceBinding = renderPipeline->CreateResourceBinding();

    vs->release();
    ps->release();

    RHITextureDescriptor textureDesc{};
    textureDesc.resourceType = RHIResourceType::texture2D;
    textureDesc.format = RHIFormat::B8G8R8A8_srgb;
    textureDesc.width = 800;
    textureDesc.height = 800;
    textureDesc.depthOrArraySize = 1;
    textureDesc.miplevels = 1;
    textureDesc.usage = (int)RHIResourceUsageBits::allow_render_target | (int)RHIResourceUsageBits::allow_transfer_src;
    textureDesc.memoryType = RHIMemoryType::default_;
    rtTexture = device->CreateTexture(textureDesc);

    RHITextureViewDescriptor textureViewDesc{};
    textureViewDesc.texture = rtTexture;
    textureViewDesc.format = RHIFormat::B8G8R8A8_srgb;
    rtTextureView = device->CreateTextureView(textureViewDesc);

    RHIRenderPassDescriptor renderpassDesc{};
    renderpassDesc.colorAttachmentCount = 1;
    renderpassDesc.colorAttachments[0] = rtTextureView;
    renderPass = device->CreateRenderPass(renderpassDesc);

    srvTexture = createTexture("D:/workspace/projects/vulkanRenderSample/resource/sjy.png");
    RHITextureViewDescriptor textureViewDesc1{};
    textureViewDesc1.texture = srvTexture;
    textureViewDesc1.format = RHIFormat::R8G8B8A8_srgb;
    srvTextureView = device->CreateTextureView(textureViewDesc1);

    resourceBinding->SetTextureView(RHIShaderType::fragment, RHIName("sampler_tex"), srvTextureView);
}

void render()
{
    context->BeginPass(renderPass);
    context->SetRenderPipeline(renderPipeline);
    context->SetResourceBinding(resourceBinding);
    context->Draw(3, 1, 0, 0);
    context->EndPass();
    for (int i = 0; i < countOfWindows; i++)
    {
        if (windows[i])
        {
            context->CopyTexture2D(rtTexture, windows[i]->m_swapchain->GetBackTexture());
            break;
        }
    }
    
    for (int i = 0; i < countOfWindows; i++)
    {
        if (windows[i])
        {
            context->Present(windows[i]->m_swapchain, false);
        }
    }
    
}


int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    initRender();

    for (int i = 0; i < countOfWindows; i++)
    {
        windows[i] = new Window();
        windows[i]->create();
    }

    bool shouldClose = false;

    while (!shouldClose)
    {
        for (int i = 0; i < countOfWindows; i++)
        {
            if (windows[i] && glfwWindowShouldClose(windows[i]->m_window))
            {
                windows[i]->destroy();
                delete windows[i];
                windows[i] = nullptr;
            }
        }

        shouldClose = true;
        for (int i = 0; i < countOfWindows; i++)
        {
            if (windows[i])
            {
                shouldClose = false;
            }
        }

        if (!shouldClose)
        {
            render();
            glfwPollEvents();
        }
    }
    return 0;
}