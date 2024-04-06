#pragma once
#include "yjw_rhi_object.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_define.h"
#include "yjw_rhi_common.h"
#include <string>
namespace rhi
{
    class RHIResourceBinding : public RHIObject
    {
    public:
        virtual void SetTextureView(RHIShaderType shaderType, RHIName name, RHITextureView* view) = 0;
        virtual void SetBufferView(RHIShaderType shaderType, RHIName name, RHIBufferView* view) = 0;
    };
}