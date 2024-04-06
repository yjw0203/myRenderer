#pragma once
#include "yjw_rhi_object.h"
#include "yjw_rhi_resource.h"

namespace rhi
{
    class RHISwapChain : public RHIObject
    {
    public:
        virtual RHITexture* GetBackTexture() = 0;
        virtual RHITextureView* GetBackTextureView() = 0;
    };
}