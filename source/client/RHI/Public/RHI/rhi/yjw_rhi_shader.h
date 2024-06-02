#pragma once
#include "RHI/rhi/yjw_rhi_object.h"
#include "RHI/shaderCompiler/yjw_shader_compiler.h"

namespace rhi
{
    class RHIShader : public RHIObject
    {
    public:
        virtual ShaderReflect* GetShaderReflect() = 0;
    private:

    };
}