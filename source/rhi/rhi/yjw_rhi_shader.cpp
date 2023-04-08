#include "yjw_rhi_shader.h"
#include "yjw_rhi_interface.h"

namespace rhi
{
    RHIShader::RHIShader(RHIShaderType type, const char* filePath, const char* entry)
        : rhiShaderDesc(type, filePath, entry)
    {
        shaderLocation = IRHI::Get()->createShader(rhiShaderDesc);
    }

    RHIShader::~RHIShader()
    {
        IRHI::Get()->deleteShader(shaderLocation);
    }
}