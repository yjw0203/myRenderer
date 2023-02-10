#include "rhi/yjw_rhi.h"

#include "yjw_vulkan_delegate.h"
#include "yjw_vulkan_context.h"
#include <stdexcept>

#include "vulkan/vulkan.h"

namespace rhi
{
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

    void rhiBeginFrame()
    {
        vkWaitForFences(g_context.device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(g_context.device, 1, &inFlightFence);
        vkAcquireNextImageKHR(g_context.device, g_context.swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &g_context.swapchainImageIndex);

        g_context.commandBufferList.clear();
    }

	void rhiEndFrame()
	{

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = g_context.commandBufferList.size();
        submitInfo.pCommandBuffers = g_context.commandBufferList.data();

        VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(g_context.graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { g_context.swapchain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &g_context.swapchainImageIndex;

        vkQueuePresentKHR(g_context.presentQueue, &presentInfo);
	}

    void initializeSwapchain()
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(g_context.device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(g_context.device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(g_context.device, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }

    REGISTER_DELEGATE(OnRHIInitializedDelegate, initializeSwapchain)
}