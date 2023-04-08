#pragma once
#include <string>

namespace rhi
{
    enum RHIShaderType
    {
        vertex_shader,
        pixel_shader,
        compute_shader
    };

    struct RHIShaderDesc
    {
        RHIShaderType type;
        char filePath[128];
        char entry[32];
        RHIShaderDesc(RHIShaderType in_type, const char* in_filePath, const char* in_entry) : type(in_type)
        {
            strcpy(filePath, in_filePath);
            strcpy(entry, in_entry);
        }
    };

    class RHIShaderLocation{};

    class RHIShader
    {
    public:
        RHIShader(RHIShaderType type, const char* filePath, const char* entry);
        virtual ~RHIShader();
        const RHIShaderDesc rhiShaderDesc;
        RHIShaderLocation* shaderLocation = nullptr;
    };

}