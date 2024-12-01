#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_layer.h"
#include "Engine/RHI/Private/externs/imgui/native/imgui.h"
#include "Engine/RHI/Private/externs/imgui/native/imgui_impl_vulkan.h"
#include "Engine/RHI/Private/externs/imgui/native/imgui_impl_glfw.h"

#include "Engine/RHI/Private/vulkan/yjw_vulkan_context.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_instance.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_command_queue.h"

#include "Engine/RHI/Public/rpi/yjw_rpi.h"

namespace rhi
{
    RHIImguiLayer::RHIImguiLayer(ERHIType rhiType)
        :RHILayer(rhiType)
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }

    void RHIImguiLayer::OnInstanceInit(class RHIInstanceImpl* instance)
    {
    }

    void RHIImguiLayer::Init(class RHIDevice* device)
    {
        m_device = device;

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

    void RHIImguiLayer::OnSwapchainInit(class RHISwapChain* swapchain)
    {
        ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)swapchain->GetNativeWindow(), true);
        Init(swapchain->GetRHIDevice());
    }
    void RHIImguiLayer::OnSwapchainShutdown(class RHISwapChain* swapchain)
    {
        ImGui_ImplGlfw_Shutdown();
    }

    void RHIImguiLayer::NewFrame(RHIContext* context, RHIRenderPass* renderPass)
    {
        VulkanContext* vkContext = (VulkanContext*)context;
        for (auto& iter : m_registered_textures)
        {
            ((VulkanTextureView*)iter.second.m_texture_view)->GetTexture()->TransitionState(vkContext->GetVkCommandBuffer(), VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }
        context->BeginPass(renderPass);
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(renderPass->GetWidth(), renderPass->GetHeight());;
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void RHIImguiLayer::Render(RHIContext* context)
    {
        VulkanContext* vkContext = (VulkanContext*)context;
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), vkContext->GetVkCommandBuffer());
        context->EndPass();
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void* RHIImguiLayer::RegisterTexture(const char* name, RHITextureView* texture)
    {
        VulkanDevice* vkDevice = (VulkanDevice*)m_device;
        VkTextureId id{};
        id.m_texture_view = texture;
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
        vkCreateDescriptorPool(vkDevice->GetNativeDevice(), &pool_info, nullptr, &id.m_descriptor_pool);

        VkDescriptorSetLayoutBinding binding;
        binding.descriptorCount = 1;
        binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        binding.binding = 0;
        binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        binding.pImmutableSamplers = nullptr;

        VkDescriptorSetLayoutCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.bindingCount = 1;
        createInfo.pBindings = &binding;
        createInfo.flags = 0;
        createInfo.pNext = nullptr;
        vkCreateDescriptorSetLayout(vkDevice->GetNativeDevice(), &createInfo, nullptr, &id.m_descriptor_set_layout);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = id.m_descriptor_pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &id.m_descriptor_set_layout;
        vkAllocateDescriptorSets(vkDevice->GetNativeDevice(), &allocInfo, &id.m_texture_id);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = VKResourceCast(texture)->GetVkImageView();
        imageInfo.sampler = vkDevice->m_default_sampler;

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = id.m_texture_id;
        write.dstBinding = 0;
        write.dstArrayElement = 0;
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.descriptorCount = 1;
        write.pImageInfo = &imageInfo;
        vkUpdateDescriptorSets(vkDevice->GetNativeDevice(), 1, &write, 0, nullptr);
        m_registered_textures[std::string(name)] = id;
        return id.m_texture_id;
    }

    void* RHIImguiLayer::GetImTextureID(const char* name)
    {
        return (void*)m_registered_textures[std::string(name)].m_texture_id;
    }
}