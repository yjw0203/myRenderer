#include "yjw_vulkan_command_list.h"
#include "yjw_vulkan_command_queue.h"
#include "vulkan/vulkan.h"
#include "yjw_vulkan_resource_cast.h"

namespace rhi
{
    VulkanCommandList::VulkanCommandList(VulkanDevice* device)
        :VulkanDeviceObject(device)
    {
        VkCommandPoolCreateInfo command_pool_info{};
        command_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        command_pool_info.queueFamilyIndex = GetDevice()->GetQueueFamilyIndices().graphicsFamily.value();
        command_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        command_pool_info.pNext = nullptr;
        vkCreateCommandPool(GetDevice()->GetNativeDevice(), &command_pool_info, nullptr, &m_command_pool);

        VkCommandBufferAllocateInfo command_buffer_info{};
        command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        command_buffer_info.commandPool = m_command_pool;
        command_buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        command_buffer_info.commandBufferCount = k_max_command_buffer_count;
        command_buffer_info.pNext = nullptr;
        vkAllocateCommandBuffers(GetDevice()->GetNativeDevice(), &command_buffer_info, m_command_buffer);
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        for (int index = 0; index < k_max_command_buffer_count; index++)
        {
            vkCreateFence(GetDevice()->GetNativeDevice(), &fenceInfo, nullptr, &m_fence[index]);
        }

        NextCommandBuffer();
    }

    VulkanCommandList::~VulkanCommandList()
    {
        for (int index = 0; index < k_max_command_buffer_count; index++)
        {
            vkDestroyFence(GetDevice()->GetNativeDevice(), m_fence[index], nullptr);
        }
        vkFreeCommandBuffers(GetDevice()->GetNativeDevice(), m_command_pool, k_max_command_buffer_count, m_command_buffer);
        vkDestroyCommandPool(GetDevice()->GetNativeDevice(), m_command_pool, nullptr);
    }

    VkCommandBuffer VulkanCommandList::GetCommandBuffer()
    {
        return m_command_buffer[m_current_command_buffer_index];
    }

    void VulkanCommandList::Submit()
    {
        vkEndCommandBuffer(m_command_buffer[m_current_command_buffer_index]);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_command_buffer[m_current_command_buffer_index];
        vkQueueSubmit(GetDevice()->GetCommandQueue()->GetGraphicsQueue(), 1, &submitInfo, m_fence[m_current_command_buffer_index]);

        NextCommandBuffer();
    }

    void VulkanCommandList::NextCommandBuffer()
    {
        m_current_command_buffer_index = (m_current_command_buffer_index + 1) % k_max_command_buffer_count;

        vkWaitForFences(GetDevice()->GetNativeDevice(), 1, &m_fence[m_current_command_buffer_index], VK_TRUE, UINT64_MAX);
        vkResetFences(GetDevice()->GetNativeDevice(), 1, &m_fence[m_current_command_buffer_index]);
        vkResetCommandBuffer(m_command_buffer[m_current_command_buffer_index], VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(m_command_buffer[m_current_command_buffer_index], &beginInfo);
    }

    VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice* device, VulkanStateCache& stateCache)
        :VulkanDeviceObject(device),
        m_command_list(device),
        m_state_cache(stateCache)
    {}

    void VulkanCommandBuffer::CmdBeginPass(VulkanRenderPass* renderPass)
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass->GetNativeRenderPass();
        renderPassInfo.framebuffer = renderPass->GetFramebuffer();
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = VkExtent2D{ (uint32_t)renderPass->GetWidth(), (uint32_t)renderPass->GetHeight() };
        renderPassInfo.clearValueCount = 0;
        renderPassInfo.pClearValues = nullptr;
        renderPass->TransitionStateToRender(m_command_list.GetCommandBuffer());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = renderPass->GetWidth();
        viewport.height = renderPass->GetHeight();
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_command_list.GetCommandBuffer(), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = VkExtent2D{ (uint32_t)renderPass->GetWidth() ,(uint32_t)renderPass->GetHeight() };
        vkCmdSetScissor(m_command_list.GetCommandBuffer(), 0, 1, &scissor);

        vkCmdBeginRenderPass(m_command_list.GetCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        m_current_render_pass = renderPass;
    }

    void VulkanCommandBuffer::CmdEndPass()
    {
        vkCmdEndRenderPass(m_command_list.GetCommandBuffer());
    }

    void VulkanCommandBuffer::CmdDraw(int vertexCount, int instanceCount, int firstVertex, int firstInstance)
    {
        PrepareForRender();
        vkCmdDraw(m_command_list.GetCommandBuffer(), vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void VulkanCommandBuffer::Submit()
    {
        m_command_list.Submit();
    }

    void VulkanCommandBuffer::Present(VulkanSwapChain* swapchain, bool bSync)
    {
        ResourceCast(swapchain->GetBackTexture())->TransitionState(m_command_list.GetCommandBuffer(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
        Submit();
        swapchain->Present(bSync);
    }

    void VulkanCommandBuffer::CopyTexture2D(VulkanTexture* srcTexture, VulkanTexture* dstTexture)
    {
        VkImageCopy imageCopyRegion{};
        imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageCopyRegion.srcSubresource.layerCount = 1;
        imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageCopyRegion.dstSubresource.layerCount = 1;
        imageCopyRegion.extent.width = srcTexture->GetWidth();
        imageCopyRegion.extent.height = srcTexture->GetHeight();
        imageCopyRegion.extent.depth = 1;
        srcTexture->TransitionState(m_command_list.GetCommandBuffer(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        dstTexture->TransitionState(m_command_list.GetCommandBuffer(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        vkCmdCopyImage(m_command_list.GetCommandBuffer(), srcTexture->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstTexture->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);
    }

    void VulkanCommandBuffer::PrepareForRender()
    {
        vkCmdBindPipeline(m_command_list.GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_state_cache.GetRenderPipeline()->GetOrCreateVkPipeline(m_current_render_pass));
        vkCmdSetPrimitiveTopology(m_command_list.GetCommandBuffer(), VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    }
}