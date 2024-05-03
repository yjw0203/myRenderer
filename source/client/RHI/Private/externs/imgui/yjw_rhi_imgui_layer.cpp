#include "Public/RHI/externs/imgui/yjw_rhi_imgui_layer.h"
#include "Private/externs/imgui/native/imgui.h"
#include "Private/externs/imgui/native/imgui_impl_vulkan.h"

#include "Private/vulkan/yjw_vulkan_context.h"
#include "Private/vulkan/yjw_vulkan_instance.h"
#include "Private/vulkan/yjw_vulkan_device.h"
#include "Private/vulkan/yjw_vulkan_command_queue.h"

#include "Public/RHI/rpi/yjw_rpi.h"

namespace rhi
{
    RHIImguiLayer::RHIImguiLayer(ERHIType rhiType)
        :RHILayer(rhiType)
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    }

    void RHIImguiLayer::OnInstanceInit(class RHIInstanceImpl* instance)
    {
    }

    void RHIImguiLayer::OnDeviceInit(class RHIDevice* device)
    {
        VulkanDevice* vkDevice = (VulkanDevice*)device;

        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1;
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        vkCreateDescriptorPool(vkDevice->GetNativeDevice(), &pool_info, nullptr, &m_descriptor_pool);

        RHITextureDescriptor desc{};
        desc.resourceType = RHIResourceType::texture2D;
        desc.width = 1200;
        desc.height = 1200;
        desc.depthOrArraySize = 1;
        desc.miplevels = 1;
        desc.format = rpi::RPIFormat::R8G8B8A8_unorm;
        desc.usage = (int)RHIResourceUsageBits::allow_render_target | (int)RHIResourceUsageBits::allow_transfer_src | (int)RHIResourceUsageBits::allow_transfer_dst | (int)RHIResourceUsageBits::allow_unordered_access;
        desc.memoryType = RHIMemoryType::default_;
        m_texture = device->CreateTexture(desc);

        RHITextureViewDescriptor viewDesc{};
        viewDesc.texture = m_texture;
        viewDesc.format = rpi::RPIFormat::R8G8B8A8_unorm;
        m_texture_view = device->CreateTextureView(viewDesc);

        RHIRenderPassDescriptor renderPassDesc{};
        renderPassDesc.colorAttachments[0] = m_texture_view;
        renderPassDesc.colorAttachmentCount = 1;
        renderPassDesc.depthStencilAttachment = nullptr;
        m_render_pass = device->CreateRenderPass(renderPassDesc);

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable some options

        ImGui_ImplVulkan_InitInfo init_info{};
        init_info.Instance = vkDevice->GetParentInstance()->GetNativeInstance();
        init_info.PhysicalDevice = vkDevice->GetGpu();
        init_info.Device = vkDevice->GetNativeDevice();
        init_info.QueueFamily = vkDevice->GetQueueFamilyIndices().graphicsFamily.value();
        init_info.Queue = vkDevice->GetCommandQueue()->GetGraphicsQueue();
        init_info.DescriptorPool = m_descriptor_pool;
        init_info.RenderPass = ((VulkanRenderPass*)m_render_pass)->GetNativeRenderPass();
        init_info.MinImageCount = 2;
        init_info.ImageCount = 2;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.UseDynamicRendering = false;

        ImGui_ImplVulkan_Init(&init_info);
    }

    void RHIImguiLayer::OnDeviceShutdown(class RHIDevice* device)
    {
        VulkanDevice* vkDevice = (VulkanDevice*)device;

        ImGui_ImplVulkan_Shutdown();
        ImGui::DestroyContext();
        vkDestroyDescriptorPool(vkDevice->GetNativeDevice(), m_descriptor_pool, nullptr);
    }

    void RHIImguiLayer::NewFrame(RHIContext* context, RHIRenderPass* renderPass)
    {
        context->BeginPass(renderPass);
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(renderPass->GetWidth(), renderPass->GetHeight());;
        ImGui_ImplVulkan_NewFrame();
        ImGui::NewFrame();
    }

    void RHIImguiLayer::Render(RHIContext* context)
    {
        VulkanContext* vkContext = (VulkanContext*)context;
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), vkContext->GetVkCommandBuffer());
        context->EndPass();
    }
}