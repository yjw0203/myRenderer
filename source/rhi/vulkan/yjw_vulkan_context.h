#pragma once
#include "RHI/rhi/yjw_rhi_header.h"
#include "yjw_vulkan_state_cache.h"
#include "yjw_vulkan_render_pass.h"
#include "yjw_vulkan_device.h"
#include "yjw_vulkan_command_list.h"

namespace rhi
{
    class VulkanContext : public RHIContext, VulkanDeviceObject
    {
    public:
        VulkanContext(VulkanDevice* device);
        virtual void BeginPass(RHIRenderPass* renderPass) override;
        virtual void EndPass() override;
        virtual void SetRenderPipeline(RHIRenderPipeline* pipeline) override;
        virtual void Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance) override;
        virtual void Submit() override;
        virtual void Present(RHISwapChain* swapchain, bool bSync) override;

        virtual void CopyTexture2D(RHITexture* srcTexture, RHITexture* dstTexture) override;
    private:
        VulkanStateCache m_state_cache;
        VulkanCommandBuffer m_command_buffer;
    };
}