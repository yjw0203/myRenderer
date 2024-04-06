#include "yjw_vulkan_swap_chain.h"
#include "yjw_vulkan_command_queue.h"
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <algorithm>

namespace rhi
{
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,VkSurfaceKHR surface) {
        SwapChainSupportDetails details{};

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
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

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,void* window) {
        if (capabilities.currentExtent.width != UINT_MAX) {
            return capabilities.currentExtent;
        }
        else {
            int width, height;
            glfwGetFramebufferSize((GLFWwindow*)window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    VulkanSwapChain::VulkanSwapChain(VulkanDevice* pDevice, void* window)
        :VulkanDeviceObject(pDevice)
    {
        VkResult re;
        if ((re = glfwCreateWindowSurface(pDevice->GetParentInstance()->GetNativeInstance(), (GLFWwindow*)window, nullptr, &m_surface)) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }

        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(pDevice->GetGpu(), m_surface);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = { pDevice->GetQueueFamilyIndices().graphicsFamily.value(), pDevice->GetQueueFamilyIndices().presentFamily.value() };

        if (pDevice->GetQueueFamilyIndices().graphicsFamily != pDevice->GetQueueFamilyIndices().presentFamily) {
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

        if ((re = vkCreateSwapchainKHR(pDevice->GetNativeDevice(), &createInfo, nullptr, &m_native_swapchain)) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }

        m_swapchainImageFormat = surfaceFormat.format;
        m_swapchainExtent = extent;

        //create swapchain image
        std::vector<VkImage> images;
        vkGetSwapchainImagesKHR(pDevice->GetNativeDevice(), m_native_swapchain, &imageCount, nullptr);
        images.resize(imageCount);
        vkGetSwapchainImagesKHR(pDevice->GetNativeDevice(), m_native_swapchain, &imageCount, images.data());


        //create swapchain image view
        m_swapchainImages.resize(imageCount);
        m_swapchainImageViews.resize(imageCount);

        for (size_t i = 0; i < imageCount; i++) 
        {
            RHITextureDescriptor textureDesc{};
            textureDesc.resourceType = RHIResourceType::texture2D;
            textureDesc.format = RHIFormat::B8G8R8A8_srgb;
            textureDesc.width = m_swapchainExtent.width;
            textureDesc.height = m_swapchainExtent.height;
            textureDesc.miplevels = 1;
            textureDesc.depthOrArraySize = 1;
            textureDesc.usage = (int)RHIResourceUsageBits::allow_render_target | (int)RHIResourceUsageBits::allow_transfer_src | (int)RHIResourceUsageBits::allow_transfer_dst;
            textureDesc.memoryType = RHIMemoryType::default_;
            m_swapchainImages[i] = new VulkanTexture(pDevice, textureDesc, images[i], VK_IMAGE_LAYOUT_UNDEFINED);

            RHITextureViewDescriptor viewDesc{};
            viewDesc.texture = m_swapchainImages[i];
            viewDesc.format = RHIFormat::B8G8R8A8_srgb;
            m_swapchainImageViews[i] = new VulkanTextureView(pDevice, viewDesc);
        }

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        if (vkCreateSemaphore(pDevice->GetNativeDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }

        vkAcquireNextImageKHR(GetDevice()->GetNativeDevice(), m_native_swapchain, UINT64_MAX, m_imageAvailableSemaphore, VK_NULL_HANDLE, &m_swapchainImageIndex);
    }

    VulkanSwapChain::~VulkanSwapChain()
    {
        vkDestroySemaphore(GetDevice()->GetNativeDevice(), m_imageAvailableSemaphore, nullptr);
        vkDestroySwapchainKHR(GetDevice()->GetNativeDevice(), m_native_swapchain, nullptr);
    }

    void VulkanSwapChain::Present(bool bSync)
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;;
        presentInfo.pWaitSemaphores = &m_imageAvailableSemaphore;

        VkSwapchainKHR swapChains[] = { m_native_swapchain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &m_swapchainImageIndex;

        vkQueuePresentKHR(GetDevice()->GetCommandQueue()->GetPresentQueue(), &presentInfo);
        vkAcquireNextImageKHR(GetDevice()->GetNativeDevice(), m_native_swapchain, UINT64_MAX, m_imageAvailableSemaphore, VK_NULL_HANDLE, &m_swapchainImageIndex);
    }

    RHITexture* VulkanSwapChain::GetBackTexture()
    {
        return m_swapchainImages[m_swapchainImageIndex];
    }

    RHITextureView* VulkanSwapChain::GetBackTextureView()
    {
        return m_swapchainImageViews[m_swapchainImageIndex];
    }
}