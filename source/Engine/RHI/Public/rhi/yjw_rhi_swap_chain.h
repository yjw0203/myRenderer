#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_object.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_resource.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_render_pass.h"

namespace rhi
{
    class RHISwapChain : public RHIObject
    {
    public:
        virtual RHITexture* GetBackTexture() = 0;
        virtual RHITextureView* GetBackTextureView() = 0;
        virtual RHIRenderPass* GetCurrentRenderPass() = 0;
        virtual void* GetNativeWindow() = 0;
        virtual class RHIDevice* GetRHIDevice() = 0;
    };
}