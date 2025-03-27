#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_object.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_shader.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_define.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_resource_binding.h"
#include <string>
namespace rhi
{
    class RHIPipeline : public RHIObject
    {
    public:
        virtual RHIPrimitiveBinding* CreatePrimitiveBinding() = 0;
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

    class RHIComputePipeline : public RHIPipeline
    {
    public:
        RHIComputePipeline(const RHIComputePipelineDescriptor& desc) : m_descriptor(desc) {}
        virtual ShaderReflect* GetVSShaderReflect() override { return nullptr; }
        virtual ShaderReflect* GetPSShaderReflect() override { return nullptr; }
        virtual RHIPrimitiveBinding* CreatePrimitiveBinding() override { return nullptr; }
    public:
        RHIComputePipelineDescriptor m_descriptor{};
    };
}