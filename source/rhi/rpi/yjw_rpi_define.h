#pragma once
#include "rhi/rhi/yjw_rhi_header.h"

namespace rpi
{
    using namespace rhi;
#define RPINull nullptr

    typedef RHIInstance* RPIInstance;
    typedef RHIDevice* RPIDevice;
    typedef RHIContext* RPIContext;
    typedef RHIBuffer* RPIBuffer;
    typedef RHITexture* RPITexture;
    typedef RHIBufferView* RPIBufferView;
    typedef RHITextureView* RPITextureView;
    typedef RHIPipeline* RPIPipeline;
    typedef RHIRenderPass* RPIRenderPass;
    typedef RHIResourceBinding* RPIResourceBinding;
    typedef RHIShader* RPIShader;
    typedef RHISwapChain* RPISwapChain;

    typedef RHIFormat RPIFormat;
    typedef RHIShaderType RPIShaderType;

    typedef RHIRenderPipelineDescriptor RPIRenderPipelineDescriptor;

    struct RPIWindow
    {
        void* window;
        RPISwapChain swapchain;
    };

    enum RPIDepthStencilState
    {
        no_depth_no_stencil,
        depth_read_and_wirte,
        depth_read,
        depth_write
    };

    enum RPIRasterizationState
    {
        default_,
        only_back_face,
        both_face
    };

    struct RPI
    {
        RPIInstance instance;
        RPIDevice device;
    };
}