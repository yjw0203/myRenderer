#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_object.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_shader.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_define.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_common.h"
#include <string>
namespace rhi
{
    class RHIResourceSet : public RHIObject
    {
    public:
        virtual ~RHIResourceSet() {};
        virtual void SetTextureView(RHIName name, RHITextureView* view) = 0;
        virtual void SetBufferView(RHIName name, RHIBufferView* view) = 0;
    };

    class RHIResourceBinding : public RHIObject
    {
    public:
        virtual ~RHIResourceBinding() {};
        virtual void SetResourceSet(int set_id, RHIResourceSet* set) = 0;
    };

    class RHIPrimitiveBinding : public RHIObject
    {
    public:
        virtual void SetVertexBuffer(RHIName name, RHIBufferView* buffer) = 0;
        virtual void SetIndexBuffer(RHIBufferView* buffer, int intdex_start, int index_count, bool is_index_16bit) = 0;
    };
}