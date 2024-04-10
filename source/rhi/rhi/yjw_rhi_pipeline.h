#pragma once
#include "yjw_rhi_object.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_define.h"
#include <string>
#include "yjw_rhi_resource_binding.h"
namespace rhi
{
    class RHIPipeline
    {
    public:
        virtual RHIResourceBinding* CreateResourceBinding() = 0;
    };

    class RHIRenderPipeline : public RHIPipeline, public RHIObject
    {
    public:
        RHIRenderPipeline(const RHIRenderPipelineDescriptor& desc) : m_descriptor(desc) {}
        virtual RHIResourceBinding* CreateResourceBinding() = 0;
    public:
        RHIRenderPipelineDescriptor m_descriptor{};
    };
}