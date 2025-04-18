#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_header.h"

namespace rpi
{
    using namespace rhi;
#define RPINull nullptr

    typedef RHIInstance* RPIInstance;
    typedef RHIDevice* RPIDevice;
    typedef RHIContext* RPIContext;
    typedef RHIComputePipeline* RPIComputePipeline;
    typedef RHIRenderPass* RPIRenderPass;
    typedef RHIShader* RPIShader;
    typedef ShaderReflect* RPIShaderReflect;
    typedef RHISwapChain* RPISwapChain;
    typedef RHIFence* RPIFence;
    typedef RHIFormat RPIFormat;
    typedef RHIShaderType RPIShaderType;

    typedef RHIComputePipelineDescriptor RPIComputePipelineDescriptor;
    typedef RHIDepthStencilState RPIDepthStencilState;
    typedef RHIColorBlendState RPIColorBlendState;

    struct RPIWindow
    {
        void* window;
        RPISwapChain swapchain;
    };

    struct RPI
    {
        RPIInstance instance;
        RPIDevice device;
    };
}

#include "yjw_rpi_resource.h"
#include "yjw_rpi_resource_binding.h"