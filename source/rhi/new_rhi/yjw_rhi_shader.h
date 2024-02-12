#pragma once
#include "yjw_rhi_define.h"
#include "yjw_rhi_object.h"
#include <string>
#include "rhi/common/yjw_rhi_resource_allocator.h"
namespace rhi
{
    typedef RHIHandle RHIShaderHandle;

    struct RHIShaderCreation
    {
        std::string name;
    };

    struct ShaderEntry
    {
        ShaderType shaderType;
        RHIShaderHandle shader;
        std::string entryName;
    };
}