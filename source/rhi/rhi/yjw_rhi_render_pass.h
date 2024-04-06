#pragma once
#include "yjw_rhi_object.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_define.h"

namespace rhi
{
    class RHIRenderPass : public RHIObject
    {
    public:
        RHIRenderPass(const RHIRenderPassDescriptor& desc) : m_descriptor(desc) {};
        const RHIRenderPassDescriptor& GetDesc() { return m_descriptor; }
    private:
        const RHIRenderPassDescriptor m_descriptor;
    };
}