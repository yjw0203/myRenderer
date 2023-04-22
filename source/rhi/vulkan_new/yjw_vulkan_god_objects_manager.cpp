#include "yjw_vulkan_god_objects_manager.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "yjw_vulkan_type_conversion.h"
#include "yjw_vulkan_resource_view_manager.h"
#include "yjw_vulkan_resource_manager.h"
#include "yjw_vulkan_pipeline_state.h"
#include "yjw_vulkan_shader_manager.h"
#include "yjw_vulkan_command_buffer_manager.h"
#include "yjw_vulkan_command_buffer_manager.h"

#include <stdexcept>
#include <vector>
#include <set>
#include <algorithm>
#include <array>

namespace rhi
{
    VulkanGod vulkanGod{};

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void logSupportLayers();
    void createInstance();
    void createSurface();
    void createPhysicalDevice();
    void createLogicalDeviceAndQueues();
    void createSwapchain();
    void initializeSwapchain();
    void createDefaultSampler();
    std::vector<const char*> getRequiredExtensions();

    void VulkanGod::initialize(CreateInfo info)
    {
        createInfo = info;
        logSupportLayers();
        createInstance();
        createSurface();
        createPhysicalDevice();
        createLogicalDeviceAndQueues();
        VulkanCommandBufferAllocater::Get().initialize();
        createSwapchain();
        initializeSwapchain();
        createDefaultSampler();
    }

    void VulkanGod::beginFrame()
    {
        vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(device, 1, &inFlightFence);
        vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &swapchainImageIndex);
        
        VulkanCommandBufferAllocater::Get().resetOneTimeCommandBuffer();
        commandBufferList.clear();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().beginOneTimeCommandBuffer();
        transitionImageLayout(commandBuffer, vulkanGod.swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        VulkanCommandBufferAllocater::Get().endOneTimeCommandBuffer(commandBuffer);
    }

    void VulkanGod::endFrame(RHIResource* present_texture)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().beginOneTimeCommandBuffer();

        if (present_texture)
        {
            VkImage present_image = *((VulkanResourceLocation*)(present_texture->resourceLocation))->getVkImage();
            VkImageCopy imageCopyRegion{};
            imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageCopyRegion.srcSubresource.layerCount = 1;
            imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageCopyRegion.dstSubresource.layerCount = 1;
            imageCopyRegion.extent.width = vulkanGod.swapchainExtent.width;
            imageCopyRegion.extent.height = vulkanGod.swapchainExtent.height;
            imageCopyRegion.extent.depth = 1;
            transitionImageLayout(commandBuffer, present_image, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
            vkCmdCopyImage(commandBuffer, present_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vulkanGod.swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);
            transitionImageLayout(commandBuffer, present_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        }
        transitionImageLayout(commandBuffer, vulkanGod.swapchainImages[swapchainImageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
        
        VulkanCommandBufferAllocater::Get().endOneTimeCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = commandBufferList.size();
        submitInfo.pCommandBuffers = commandBufferList.data();

        VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { swapchain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &swapchainImageIndex;

        vkQueuePresentKHR(presentQueue, &presentInfo);
    }

    const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    void createInstance()
    {
        //create instance
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        std::vector<const char*> extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = (uint32_t)extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();

        //debug layer
        if (vulkanGod.isDebugMode)
        {
            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &vulkanGod.instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }

    void createSurface()
    {
        //create surface
        VkResult re;
        if ((re = glfwCreateWindowSurface(vulkanGod.instance, (GLFWwindow*)vulkanGod.createInfo.window, nullptr, &vulkanGod.surface)) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void createPhysicalDevice()
    {
        //create physical device
        uint32_t gpuDeviceCount = 0;
        vkEnumeratePhysicalDevices(vulkanGod.instance, &gpuDeviceCount, nullptr);
        std::vector<VkPhysicalDevice> gpuList(gpuDeviceCount);
        vkEnumeratePhysicalDevices(vulkanGod.instance, &gpuDeviceCount, gpuList.data());
        vulkanGod.gpu = gpuList[0];
        vulkanGod.queueFamilyIndices = findQueueFamilies(vulkanGod.gpu);
    }

    void createLogicalDeviceAndQueues()
    {
        //create logical device and queue
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { vulkanGod.queueFamilyIndices.graphicsFamily.value(), vulkanGod.queueFamilyIndices.presentFamily.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (vulkanGod.isDebugMode) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else {
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
        }
        VkResult re = vkCreateDevice(vulkanGod.gpu, &createInfo, nullptr, &vulkanGod.device);
        if (re != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(vulkanGod.device, vulkanGod.queueFamilyIndices.graphicsFamily.value(), 0, &vulkanGod.graphicsQueue);
        vkGetDeviceQueue(vulkanGod.device, vulkanGod.queueFamilyIndices.presentFamily.value(), 0, &vulkanGod.presentQueue);
    }

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    std::vector<const char*> getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (vulkanGod.isDebugMode) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
        SwapChainSupportDetails details{};

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vulkanGod.surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkanGod.surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkanGod.surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkanGod.surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkanGod.surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != UINT_MAX) {
            return capabilities.currentExtent;
        }
        else {
            int width, height;
            glfwGetFramebufferSize((GLFWwindow*)vulkanGod.createInfo.window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void createSwapchain()
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(vulkanGod.gpu);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = vulkanGod.surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = VK_FORMAT_R8G8B8A8_SRGB;//surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = { vulkanGod.queueFamilyIndices.graphicsFamily.value(), vulkanGod.queueFamilyIndices.presentFamily.value() };

        if (vulkanGod.queueFamilyIndices.graphicsFamily != vulkanGod.queueFamilyIndices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VkResult re;
        if ((re = vkCreateSwapchainKHR(vulkanGod.device, &createInfo, nullptr, &vulkanGod.swapchain)) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }

        vulkanGod.swapchainImageFormat = surfaceFormat.format;
        vulkanGod.swapchainExtent = extent;

        //create swapchain image
        vkGetSwapchainImagesKHR(vulkanGod.device, vulkanGod.swapchain, &imageCount, nullptr);
        vulkanGod.swapchainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(vulkanGod.device, vulkanGod.swapchain, &imageCount, vulkanGod.swapchainImages.data());


        //create swapchain image view
        vulkanGod.swapchainImageViews.resize(vulkanGod.swapchainImages.size());

        for (size_t i = 0; i < vulkanGod.swapchainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = vulkanGod.swapchainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = vulkanGod.swapchainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(vulkanGod.device, &createInfo, nullptr, &vulkanGod.swapchainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views!");
            }
        }

        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().beginImmdiatelyCommandBuffer();
        for (size_t i = 0; i < vulkanGod.swapchainImages.size(); i++)
        {
            transitionImageLayout(commandBuffer, vulkanGod.swapchainImages[i], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
        }
        VulkanCommandBufferAllocater::Get().endImmdiatelyCommandBuffer(commandBuffer);


    }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vulkanGod.surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }
            if (indices.isComplete()) {
                break;
            }
            i++;
        }
        return indices;
    }

    void logSupportLayers() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (VkLayerProperties layer : availableLayers) {
            printf("Available Layer: %s\n", layer.layerName);
        }
    }

    void initializeSwapchain()
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(vulkanGod.device, &semaphoreInfo, nullptr, &vulkanGod.imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(vulkanGod.device, &semaphoreInfo, nullptr, &vulkanGod.renderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(vulkanGod.device, &fenceInfo, nullptr, &vulkanGod.inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }

    void createDefaultSampler()
    {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(vulkanGod.gpu, &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 1.0f;
        samplerInfo.mipLodBias = 0.0f;

        if (vkCreateSampler(vulkanGod.device, &samplerInfo, nullptr, &vulkanGod.defaultSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }

    VkRenderPass createRenderPass(std::vector<RHIResourceView*>& rtvs, RHIResourceView* dsv)
    {
        if (rtvs.size() == 0 && !dsv)
        {
            printf("error:there is no render targets.");
        }
        VkRenderPass renderPass;

        std::vector <VkAttachmentDescription> attachments(rtvs.size() + (dsv!=nullptr));
        for (int i = 0; i < rtvs.size(); i++)
        {
            attachments[i].format = VulkanConverter::convertFormat(rtvs[i]->format);
            attachments[i].samples = VK_SAMPLE_COUNT_1_BIT;
            attachments[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachments[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachments[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachments[i].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
        if (dsv)
        {
            attachments.back().format = VulkanConverter::convertFormat(dsv->format);
            attachments.back().samples = VK_SAMPLE_COUNT_1_BIT;
            attachments.back().loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachments.back().storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments.back().stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachments.back().stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachments.back().initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachments.back().finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = attachments.size();
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(vulkanGod.device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }

        return renderPass;
    }

    void createDescriptorLayoutSets(VkDescriptorPool& descriptorPool, std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,std::vector<VkDescriptorSet>& descriptorSets , RHIShaderView* vs, RHIShaderView* ps)
    {
        struct ShaderInfo
        {
            RHIShaderView* shader;
            VkShaderStageFlagBits stage;
        };
        std::vector<ShaderInfo> shaders = { {vs,VK_SHADER_STAGE_VERTEX_BIT},{ps,VK_SHADER_STAGE_FRAGMENT_BIT} };

        int count_set = 0;
        for (ShaderInfo& shader : shaders)
        {
            if (shader.shader)
            {
                const VulkanShaderReflectionData& reflect = ((VulkanShaderLocation*)shader.shader->getShader()->shaderLocation)->getReflectData();
                count_set += reflect.setCount;
            }
        }

        //layout
        std::vector<std::vector<VkDescriptorSetLayoutBinding>> setLayoutBindings(count_set);
        int current_set_offset = 0;
        int count_sampler2D = 0;
        for (ShaderInfo& shader : shaders)
        {
            if (shader.shader)
            {
                const VulkanShaderReflectionData& reflect = ((VulkanShaderLocation*)shader.shader->getShader()->shaderLocation)->getReflectData();
                //sampler2D
                for (uint32_t i = 0; i < reflect.sampler2Ds.size(); i++)
                {
                    VkDescriptorSetLayoutBinding layoutBinding = {};
                    layoutBinding.binding = reflect.sampler2Ds[i].binding;
                    layoutBinding.descriptorCount = 1;
                    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    layoutBinding.pImmutableSamplers = nullptr;
                    layoutBinding.stageFlags = shader.stage;
                    setLayoutBindings[current_set_offset + reflect.sampler2Ds[i].set].push_back(layoutBinding);
                    count_sampler2D++;
                }

                //uniform

                current_set_offset += reflect.setCount;
            }
        }

        descriptorSetLayouts.resize(count_set);
        for (int i = 0; i < count_set; i++)
        {
            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = setLayoutBindings[i].size();
            layoutInfo.pBindings = setLayoutBindings[i].data();

            vkCreateDescriptorSetLayout(vulkanGod.device, &layoutInfo, nullptr, &descriptorSetLayouts[i]);
        }


        //poor
        std::array<VkDescriptorPoolSize, 1> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[0].descriptorCount = count_sampler2D;
        //poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        //poolSizes[1].descriptorCount = 0;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = count_set;

        if (vkCreateDescriptorPool(vulkanGod.device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }

        //set
        descriptorSets.resize(count_set);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = descriptorSets.size();
        allocInfo.pSetLayouts = descriptorSetLayouts.data();

        if (vkAllocateDescriptorSets(vulkanGod.device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        //descriptor write
        current_set_offset = 0;
        std::vector<VkWriteDescriptorSet> descriptorWrites;
        for (ShaderInfo& shader : shaders)
        {
            if (shader.shader)
            {
                const VulkanShaderReflectionData& reflect = ((VulkanShaderLocation*)shader.shader->getShader()->shaderLocation)->getReflectData();

                //sampler2D
                for (uint32_t i = 0; i < reflect.sampler2Ds.size(); i++)
                {
                    if (!shader.shader->getData().texture2Ds.count(reflect.sampler2Ds[i].name))
                    {
                        continue;
                    }

                    VkDescriptorImageInfo imageInfo{};
                    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imageInfo.imageView = *((VulkanResourceViewLocation*)shader.shader->getData().texture2Ds[reflect.sampler2Ds[i].name]->resourceViewLocation)->getVkImageView();
                    imageInfo.sampler = vulkanGod.defaultSampler;
                    
                    VkWriteDescriptorSet write{};
                    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    write.dstSet = descriptorSets[current_set_offset + reflect.sampler2Ds[i].set];
                    write.dstBinding = reflect.sampler2Ds[i].binding;
                    write.dstArrayElement = 0;
                    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    write.descriptorCount = 1;
                    write.pImageInfo = &imageInfo;
                    descriptorWrites.push_back(write);
                }

                //uniform

                current_set_offset += reflect.setCount;
            }
        }
        vkUpdateDescriptorSets(vulkanGod.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }

    VkFramebuffer createFramebuffer(VkRenderPass renderPass, std::vector<RHIResourceView*>& rtvs, RHIResourceView* dsv, int width, int height)
    {
        VkFramebuffer frameBuffer;

        std::vector<VkImageView> attachments(rtvs.size() + (dsv!=nullptr));
        for (int i = 0; i < rtvs.size(); i++)
        {
            attachments[i] = *(((VulkanResourceViewLocation*)(rtvs[i]->resourceViewLocation))->getVkImageView());
        }
        if (dsv)
        {
            attachments.back() = *(((VulkanResourceViewLocation*)(dsv->resourceViewLocation))->getVkImageView());
        }
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = attachments.size();
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = width;
        framebufferInfo.height = height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(vulkanGod.device, &framebufferInfo, nullptr, &frameBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }

        return frameBuffer;
    }

    VkPipelineLayout createPipelineLayout(std::vector<VkDescriptorSetLayout>& descriptorSetLayouts)
    {
        VkPipelineLayout pipelineLayout;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        if (vkCreatePipelineLayout(vulkanGod.device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }

        return pipelineLayout;
    }

    VkPipeline createPipeline(VkPipelineLayout& pipelineLayout,
        VkRenderPass& renderPass,
        RasterizationState rasterizationState,
        ColorBlendState colorBlendState,
        std::vector<VkPipelineShaderStageCreateInfo>& shaderStages)
    {
        VkPipeline pipeline;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &VulkanPilelineStateManager::Get().getRasterizationState(rasterizationState);
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &VulkanPilelineStateManager::Get().getColorBlendState(colorBlendState);
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(vulkanGod.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        return pipeline;
    }

    VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(RHIShaderView* shaderView, VkShaderStageFlagBits stage)
    {
        VulkanShaderLocation* vulkanShaderLocation = (VulkanShaderLocation*)shaderView->getShader()->shaderLocation;
        VkPipelineShaderStageCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        info.stage = stage;
        info.module = vulkanShaderLocation->getShaderModule();
        info.pName = shaderView->getEntry().c_str();
        return info;
    }

    void transitionImageLayout(VkCommandBuffer& commandBuffer,VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else {
            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            //throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

    }
}
