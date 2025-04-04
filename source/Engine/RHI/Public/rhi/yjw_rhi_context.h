#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_device.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_object.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_swap_chain.h"

namespace rhi
{
    class RHIContext : public RHIObject
    {
    public:
        virtual void PushEvent(const char* name) = 0;
        virtual void PopEvent() = 0;
        virtual void BeginPass(RHIRenderPass* renderPass) = 0;
        virtual void EndPass() = 0;
        virtual void SetRenderPipeline(RHIRenderPipeline* pipeline) = 0;
        virtual void SetComputePipeline(RHIComputePipeline* pipeline) = 0;
        virtual void TransitionStateToRender(RHIResourceSet* resourceBinding) = 0;
        virtual void SetResourceBinding(RHIResourceBinding* resourceBinding) = 0;
        virtual void SetPrimitiveBinding(RHIPrimitiveBinding* resourceBinding,int sub_id) = 0;
        virtual void Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance) = 0;
        virtual void DrawIndex(int firstInstance, int instanceCount) = 0;
        virtual void Dispatch(int groupCountX, int groupCountY, int groupCountZ) = 0;
        virtual void Submit() = 0;
        virtual void Present(RHISwapChain* swapchain, bool bSync) = 0;

        virtual void CopyTexture2D(RHITexture* srcTexture, RHITexture* dstTexture) = 0;
        virtual void ClearTexture2D(RHITexture* texture) = 0;
    };
}