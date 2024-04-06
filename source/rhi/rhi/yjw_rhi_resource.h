#pragma once
#include "yjw_rhi_object.h"
#include "yjw_rhi_define.h"

namespace rhi
{
    class RHIBuffer : public RHIObject
    {
    public:
        RHIBuffer(const RHIBufferDescriptor& desc) : m_descriptor(desc) {}
        const RHIBufferDescriptor& GetDesc() { return m_descriptor; }
    private:
        const RHIBufferDescriptor m_descriptor;
    };

    class RHIBufferView : public RHIObject
    {
    public:
        RHIBufferView(const RHIBufferViewDescriptor& desc) : m_descriptor(desc) {}
        const RHIBufferViewDescriptor& GetDesc() { return m_descriptor; }
    private:
        const RHIBufferViewDescriptor m_descriptor;
    };

    class RHITexture : public RHIObject
    {
    public:
        RHITexture(const RHITextureDescriptor& desc) : m_descriptor(desc) {}
        const RHITextureDescriptor& GetDesc() { return m_descriptor; }
    private:
        const RHITextureDescriptor m_descriptor;
    };

    class RHITextureView : public RHIObject
    {
    public:
        RHITextureView(const RHITextureViewDescriptor& desc) : m_descriptor(desc) {}
        const RHITextureViewDescriptor& GetDesc() { return m_descriptor; }
    private:
        const RHITextureViewDescriptor m_descriptor;
    };

    class RHISampler : public RHIObject
    {
    };
}