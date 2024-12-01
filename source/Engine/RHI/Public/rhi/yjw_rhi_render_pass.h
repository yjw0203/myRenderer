#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_object.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_shader.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_define.h"

namespace rhi
{
    class RHIRenderPass : public RHIObject
    {
    public:
        RHIRenderPass(const RHIRenderPassDescriptor& desc) : m_descriptor(desc) {};
        const RHIRenderPassDescriptor& GetDesc() { return m_descriptor; }
        virtual int GetWidth() = 0;
        virtual int GetHeight() = 0;
    private:
        const RHIRenderPassDescriptor m_descriptor;
    };
}