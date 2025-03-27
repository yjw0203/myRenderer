#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_header.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_render_pass.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_state_cache.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_swap_chain.h"

namespace rhi
{
    class VulkanCommandList : public VulkanDeviceObject
    {
    public:
        VulkanCommandList(VulkanDevice* device);
        ~VulkanCommandList();
        VkCommandBuffer GetCommandBuffer();
        void Submit();
        void Submit(VkSubmitInfo* submitInfo);
    private:
        void NextCommandBuffer();
    private:
        VkCommandPool m_command_pool = nullptr;
        static const int k_max_command_buffer_count = 5;
        int m_current_command_buffer_index = 0;
        VkCommandBuffer m_command_buffer[k_max_command_buffer_count] = {};
        VkFence m_fence[k_max_command_buffer_count] = {};
    };

    class VulkanCommandBuffer : public VulkanDeviceObject, RHIObject
    {
    public:
        VulkanCommandBuffer(VulkanDevice* device, VulkanStateCache& stateCache);
        void CmdPushEvent(const char* name);
        void CmdPopEvent();
        void CmdBeginPass(VulkanRenderPass* renderPass);
        void CmdEndPass();
        void CmdTransitionStateToRender(VulkanResourceSet* resourceSet);
        void CmdDraw(int vertexCount, int instanceCount, int firstVertex, int firstInstance);
        void CmdDrawIndex(int firstInstance, int instanceCount);
        void CmdDispatch(int groupCountX, int groupCountY, int groupCountZ);
        void Submit();
        void Present(VulkanSwapChain* swapchain, bool bSync);

        void CopyTexture2D(VulkanTexture* srcTexture, VulkanTexture* dstTexture);
        void ClearTexture2D(VulkanTexture* texture);

        VkCommandBuffer GetCommandBuffer();
    private:
        void PrepareForRender();
        void PrepareForDispatch();
    private:
        VulkanRenderPass* m_current_render_pass = nullptr;
        VulkanStateCache& m_state_cache;
        VulkanCommandList m_command_list;
    };
}