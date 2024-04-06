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

    void VulkanContext::Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance)
    {
        m_command_buffer.CmdDraw(vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void VulkanContext::Submit()
    {
        m_command_buffer.Submit();
    }

    void VulkanContext::Present(RHISwapChain* swapchain, bool bSync)
    {
        m_command_buffer.Present(ResourceCast(swapchain), bSync);
    }
}