#pragma once
#include "yjw_rhi_device.h"
#include "yjw_rhi_object.h"
#include "yjw_rhi_swap_chain.h"

namespace rhi
{
    class RHIContext : public RHIObject
    {
    public:
        virtual void BeginPass(RHIRenderPass* renderPass) = 0;
        virtual void EndPass() = 0;
        virtual void SetRenderPipeline(RHIRenderPipeline* pipeline) = 0;
        virtual void TransitionStateToRender(RHIResourceBinding* resourceBinding) = 0;
        virtual void SetResourceBinding(RHIResourceBinding* resourceBinding) = 0;
        virtual void Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance) = 0;
        virtual void DrawIndex(int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance) = 0;
        virtual void Submit() = 0;
        virtual void Present(RHISwapChain* swapchain, bool bSync) = 0;

        virtual void CopyTexture2D(RHITexture* srcTexture, RHITexture* dstTexture) = 0;
        virtual void ClearTexture2D(RHITexture* texture) = 0;
    };
}