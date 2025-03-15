#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_swap_chain.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_texture.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_render_pass.h"
#include <vector>

namespace rhi
{
    class VulkanSwapChain : public RHISwapChain,VulkanDeviceObject
    {
    public:
        VulkanSwapChain(VulkanDevice* pDevice, void* window);
        ~VulkanSwapChain();
        virtual RHITexture* GetBackTexture() override;
        virtual RHITextureView* GetBackTextureView() override;
        virtual RHITexture* GetDepthTexture() override;
        virtual RHITextureView* GetDepthTextureView() override;
        virtual RHIRenderPass* GetCurrentRenderPass() override;
        virtual void* GetNativeWindow() override;
        virtual RHIDevice* GetRHIDevice() override;
        void Present(bool bSync);
    private:
        VkSurfaceKHR m_surface = nullptr;
        VkSwapchainKHR m_native_swapchain = nullptr;
        VkFormat m_swapchainImageFormat{};
        VkExtent2D m_swapchainExtent{};
        uint32_t m_swapchainImageIndex{};
        uint32_t m_swapchainImageCount{};
        VulkanTexture* m_depthImage{};
        VulkanTextureView* m_depthImageView{};
        std::vector<VulkanTexture*> m_swapchainImages;
        std::vector<VulkanTextureView*> m_swapchainImageViews;
        std::vector<VulkanRenderPass*> m_swapchainRenderPasses;
        std::vector<VkSemaphore> m_imageAvailableSemaphore;
        int m_maxFrameInFlight = 3;
        int m_currentFlightFrame = 0;
        void* m_window = nullptr;
    };
}