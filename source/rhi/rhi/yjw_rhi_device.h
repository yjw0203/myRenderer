#pragma once
#include "yjw_rhi_swap_chain.h"
#include "yjw_rhi_object.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_pipeline.h"
#include "yjw_rhi_render_pass.h"
#include "yjw_rhi_resource.h"

namespace rhi
{
    class RHIContext;
    class RHIDevice : public RHIObject
    {
    public:
        virtual RHIContext* CreateContext() = 0;
        virtual RHISwapChain* CreateSwapchain(void* window) = 0;
        virtual RHIShader* CreateShaderByBinaryUrl(const char* url) = 0;
        virtual RHIRenderPipeline* CreateRenderPipeline(const RHIRenderPipelineDescriptor& renderPipelineDescriptor) = 0;
        virtual RHIRenderPass* CreateRenderPass(const RHIRenderPassDescriptor& renderPassDescriptor) = 0;
        virtual RHIBuffer* CreateBuffer(const RHIBufferDescriptor& bufferDescriptor) = 0;
        virtual RHITexture* CreateTexture(const RHITextureDescriptor& tetureDescriptor) = 0;
        virtual RHIBufferView* CreateBufferView(const RHIBufferViewDescriptor& viewDescriptor) = 0;
        virtual RHITextureView* CreateTextureView(const RHITextureViewDescriptor& viewDescriptor) = 0;
    };
}