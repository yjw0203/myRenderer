#pragma once
#include "RHI/rhi/yjw_rhi_swap_chain.h"
#include "yjw_vulkan_device.h"
#include <vector>
#include "yjw_vulkan_texture.h"

namespace rhi
{
    class VulkanSwapChain : public RHISwapChain,VulkanDeviceObject
    {
    public:
        VulkanSwapChain(VulkanDevice* pDevice, void* window);
        virtual RHITexture* GetBackTexture() override;
        virtual RHITextureView* GetBackTextureView() override;
        void Present(bool bSync);
    private:
        VkSurfaceKHR m_surface = nullptr;
        VkSwapchainKHR m_native_swapchain = nullptr;
        VkFormat m_swapchainImageFormat{};
        VkExtent2D m_swapchainExtent{};
        uint32_t m_swapchainImageIndex{};
        std::vector<VulkanTexture*> m_swapchainImages;
        std::vector<VulkanTextureView*> m_swapchainImageViews;
        VkSemaphore m_imageAvailableSemaphore;
    };
}