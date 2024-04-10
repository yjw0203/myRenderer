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
        m_command_buffer.CmdBeginPass(ResourceCast(renderPass));
    }

    void VulkanContext::EndPass()
    {
        m_command_buffer.CmdEndPass();
    }

    void VulkanContext::SetRenderPipeline(RHIRenderPipeline* pipeline)
    {
        m_state_cache.SetRenderPipeline(ResourceCast(pipeline));
    }

    void VulkanContext::SetResourceBinding(RHIResourceBinding* resourceBinding)
    {
        m_state_cache.SetResourceBinding(ResourceCast(resourceBinding));
        ResourceCast(resourceBinding)->TransitionStateToRender(m_command_buffer.m_command_list.GetCommandBuffer());
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
        m_command_buffer.Present(ResourceCast(swapchain), bSync);
    }

    void VulkanContext::CopyTexture2D(RHITexture* srcTexture, RHITexture* dstTexture)
    {
        m_command_buffer.CopyTexture2D(ResourceCast(srcTexture), ResourceCast(dstTexture));
    }

    void VulkanContext::ClearTexture2D(RHITexture* texture)
    {
        m_command_buffer.ClearTexture2D(ResourceCast(texture));
    }

}