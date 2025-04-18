#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_header.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_state_cache.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_render_pass.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_command_list.h"

namespace rhi
{
    class VulkanContext : public RHIContext, VulkanDeviceObject
    {
    public:
        VulkanContext(VulkanDevice* device);
        virtual void PushEvent(const char* name) override;
        virtual void PopEvent() override;
        virtual void BeginPass(RHIRenderPass* renderPass) override;
        virtual void EndPass() override;
        virtual void SetRenderPipeline(RHIRenderPipeline* pipeline) override;
        virtual void SetComputePipeline(RHIComputePipeline* pipeline) override;
        virtual void SetPushConstants(void* data, int offset, int size) override;
        virtual void TransitionStateToRender(RHIResourceSet* resourceBinding) override;
        virtual void SetResourceSet(int set_id, RHIResourceSet* resourceSet) override;
        virtual void SetPrimitiveBinding(RHIPrimitiveBinding* resourceBinding,int sub_id) override;
        virtual void Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance) override;
        virtual void DrawIndex(int firstInstance, int instanceCount) override;
        virtual void Dispatch(int groupCountX, int groupCountY, int groupCountZ) override;
        virtual RHIFence* Submit() override;
        virtual void Present(RHISwapChain* swapchain, bool bSync) override;

        virtual void CopyTexture2D(RHITexture* srcTexture, RHITexture* dstTexture) override;
        virtual void ClearTexture2D(RHITexture* texture) override;

        VkCommandBuffer GetVkCommandBuffer();
    private:
        VulkanCommandBuffer m_command_buffer;
        VulkanStateCache m_state_cache;
    };
}