#pragma once
#include "yjw_rhi_object.h"
#include "yjw_rhi_shader.h"

namespace rhi
{
    struct RHIColorBlendState
    {

    };

    struct RHIDepthStencilState
    {
    
    };
    
    struct RHIRasterizationState
    {

    };

    class RHIRenderPipelineDescriptor
    {
    public:
        RHIColorBlendState color_blend_state{};
        RHIDepthStencilState depth_stencil_state{};
        RHIRasterizationState rasterization_state{};
        RHIShader* vs{};
        std::string vs_entry;
        RHIShader* ps{};
        std::string ps_entry;
    };

    class RHIRenderPipeline : public RHIObject
    {
    public:
        RHIRenderPipeline(const RHIRenderPipelineDescriptor& desc) : m_descriptor(desc) {}
    public:
        RHIRenderPipelineDescriptor m_descriptor;
    };
}