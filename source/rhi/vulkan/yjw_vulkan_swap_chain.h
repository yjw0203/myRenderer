#pragma once
#include "RHI/rhi/yjw_rhi_swap_chain.h"
#include "yjw_vulkan_device.h"
#include <vector>

namespace rhi
{
    class VulkanSwapChain : public RHISwapChain,VulkanDeviceObject
    {
    public:
        VulkanSwapChain(VulkanDevice* pDevice, void* window);
        virtual void Prensent(bool bSync) override;
    private:
        VkSurfaceKHR m_surface = nullptr;
        VkSwapchainKHR m_native_swapchain = nullptr;
        VkFormat m_swapchainImageFormat{};
        VkExtent2D m_swapchainExtent{};
        uint32_t m_swapchainImageIndex{};
        std::vector<VkImage> m_swapchainImages;
        std::vector<VkImageView> m_swapchainImageViews;
        VkSemaphore m_imageAvailableSemaphore;
    };
}