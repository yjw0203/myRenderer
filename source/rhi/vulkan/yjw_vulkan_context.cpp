#include "yjw_vulkan_context.h"
#include "yjw_vulkan_resource_cast.h"

namespace rhi
{
    VulkanContext::VulkanContext(VulkanDevice* device)
        :VulkanDeviceObject(device)
        , m_command_buffer(device, m_state_cache)
    {
    }

    void VulkanContext::BeginPass(RHIRenderPass* renderPass)
    {
        m_command_buffer.CmdBeginPass(VKResourceCast(renderPass));
    }

    void VulkanContext::EndPass()
    {
        m_command_buffer.CmdEndPass();
    }

    void VulkanContext::SetRenderPipeline(RHIRenderPipeline* pipeline)
    {
        m_state_cache.SetRenderPipeline(VKResourceCast(pipeline));
    }

    void VulkanContext::TransitionStateToRender(RHIResourceBinding* resourceBinding)
    {
        m_command_buffer.CmdTransitionStateToRender(VKResourceCast(resourceBinding));
    }

    void VulkanContext::SetResourceBinding(RHIResourceBinding* resourceBinding)
    {
        m_state_cache.SetResourceBinding(VKResourceCast(resourceBinding));
    }

    void VulkanContext::Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance)
    {
        m_command_buffer.CmdDraw(vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void VulkanContext::DrawIndex(int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance)
    {
        m_command_buffer.CmdDrawIndex(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    void VulkanContext::Submit()
    {
        m_command_buffer.Submit();
    }

    void VulkanContext::Present(RHISwapChain* swapchain, bool bSync)
    {
        m_command_buffer.Present(VKResourceCast(swapchain), bSync);
    }

    void VulkanContext::CopyTexture2D(RHITexture* srcTexture, RHITexture* dstTexture)
    {
        m_command_buffer.CopyTexture2D(VKResourceCast(srcTexture), VKResourceCast(dstTexture));
    }

    void VulkanContext::ClearTexture2D(RHITexture* texture)
    {
        m_command_buffer.ClearTexture2D(VKResourceCast(texture));
    }

    VkCommandBuffer VulkanContext::GetVkCommandBuffer()
    {
        return m_command_buffer.GetCommandBuffer();
    }

}