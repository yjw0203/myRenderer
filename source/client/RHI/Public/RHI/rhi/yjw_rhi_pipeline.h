#pragma once
#include "RHI/rhi/yjw_rhi_object.h"
#include "RHI/rhi/yjw_rhi_shader.h"
#include "RHI/rhi/yjw_rhi_define.h"
#include "RHI/rhi/yjw_rhi_resource_binding.h"
#include <string>
namespace rhi
{
    class RHIPipeline : public RHIObject
    {
    public:
        virtual RHIResourceBinding* CreateResourceBinding() = 0;
        virtual ShaderReflect* GetVSShaderReflect() = 0;
        virtual ShaderReflect* GetPSShaderReflect() = 0;
    };

    class RHIRenderPipeline : public RHIPipeline
    {
    public:
        RHIRenderPipeline(const RHIRenderPipelineDescriptor& desc) : m_descriptor(desc) {}
    public:
        RHIRenderPipelineDescriptor m_descriptor{};
    };
}