#include "Engine/RHI/Private/vulkan/yjw_vulkan_context.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_resource_cast.h"

namespace rhi
{
    VulkanContext::VulkanContext(VulkanDevice* device)
        :VulkanDeviceObject(device)
        , m_command_buffer(device, m_state_cache)
    {
    }

    void VulkanContext::PushEvent(const char* name)
    {
        m_command_buffer.CmdPushEvent(name);
    }

    void VulkanContext::PopEvent()
    {
        m_command_buffer.CmdPopEvent();
    }

    void VulkanContext::BeginPass(RHIRenderPass* renderPass)
    {
        m_command_buffer.CmdBeginPass(VKResourceCast(renderPass));
    }

    void VulkanContext::EndPass()
    {
        m_command_buffer.CmdEndPass();
        m_state_cache.SetResourceBinding(nullptr);
        m_state_cache.SetPrimitiveBinding(nullptr, 0);
    }

    void VulkanContext::SetRenderPipeline(RHIRenderPipeline* pipeline)
    {
        m_state_cache.SetRenderPipeline(VKResourceCast(pipeline));
    }

    void VulkanContext::SetComputePipeline(RHIComputePipeline* pipeline)
    {
        m_state_cache.SetComputePipeline(VKResourceCast(pipeline));
    }

    void VulkanContext::SetPushConstants(void* data, int offset, int size)
    {
        m_state_cache.SetPushConstants(data, offset, size);
    }

    void VulkanContext::TransitionStateToRender(RHIResourceSet* resourceBinding)
    {
        m_command_buffer.CmdTransitionStateToRender(VKResourceCast(resourceBinding));
    }

    void VulkanContext::SetResourceBinding(RHIResourceBinding* resourceBinding)
    {
        m_state_cache.SetResourceBinding(VKResourceCast(resourceBinding));
    }

    void VulkanContext::SetPrimitiveBinding(RHIPrimitiveBinding* primitiveBinding,int sub_id)
    {
        m_state_cache.SetPrimitiveBinding(VKResourceCast(primitiveBinding),sub_id);
    }

    void VulkanContext::Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance)
    {
        m_command_buffer.CmdDraw(vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void VulkanContext::DrawIndex(int firstInstance, int instanceCount)
    {
        m_command_buffer.CmdDrawIndex(firstInstance, instanceCount);
    }

    void VulkanContext::Dispatch(int groupCountX, int groupCountY, int groupCountZ)
    {
        m_command_buffer.CmdDispatch(groupCountX, groupCountY, groupCountZ);
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