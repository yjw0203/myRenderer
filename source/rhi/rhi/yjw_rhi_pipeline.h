#pragma once
#include "yjw_rhi_object.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_define.h"
#include <string>
namespace rhi
{
    class RHIRenderPipeline : public RHIObject
    {
    public:
        RHIRenderPipeline(const RHIRenderPipelineDescriptor& desc) : m_descriptor(desc) {}
    public:
        RHIRenderPipelineDescriptor m_descriptor{};
    };
}