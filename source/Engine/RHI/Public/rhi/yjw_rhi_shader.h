#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_object.h"
#include "Engine/RHI/Public/shaderCompiler/yjw_shader_compiler.h"

namespace rhi
{
    class RHIPrimitiveBinding;
    class RHIShader : public RHIObject
    {
    public:
        virtual ShaderReflect* GetShaderReflect() = 0;

        // for vertex shader
        virtual RHIPrimitiveBinding* CreatePrimitiveBinding() = 0;
    private:

    };
}