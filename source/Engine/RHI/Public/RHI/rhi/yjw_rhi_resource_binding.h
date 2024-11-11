#pragma once
#include "RHI/rhi/yjw_rhi_object.h"
#include "RHI/rhi/yjw_rhi_shader.h"
#include "RHI/rhi/yjw_rhi_define.h"
#include "RHI/rhi/yjw_rhi_common.h"
#include <string>
namespace rhi
{
    class RHIResourceBinding : public RHIObject
    {
    public:
        virtual void SetTextureView(RHIShaderType shaderType, RHIName name, RHITextureView* view) = 0;
        virtual void SetBufferView(RHIShaderType shaderType, RHIName name, RHIBufferView* view) = 0;
    };

    class RHIPrimitiveBinding : public RHIObject
    {
    public:
        virtual void SetVertexBuffer(RHIName name, RHIBufferView* buffer) = 0;
        virtual void SetIndexBuffer(RHIBufferView* buffer, int intdex_start, int index_count, bool is_index_16bit) = 0;
    };
}