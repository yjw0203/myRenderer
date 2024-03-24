#pragma once
#include "yjw_rhi_swap_chain.h"
#include "yjw_rhi_object.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_pipeline.h"

namespace rhi
{
    class RHIDevice : public RHIObject
    {
    public:
        virtual RHISwapChain* CreateSwapchain(void* window) = 0;
        virtual RHIShader* CreateShaderByBinaryUrl(const char* url) = 0;
        virtual RHIRenderPipeline* CreateRenderPipeline(const RHIRenderPipelineDescriptor& renderPipelineDescriptor) = 0;
    };
}