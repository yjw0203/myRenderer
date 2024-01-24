#pragma once
#include "yjw_rhi_define.h"
#include "yjw_rhi_object.h"
#include <string>
namespace rhi
{
    enum class ShaderType;
    class Shader;

    struct ShaderPayload
    {
        void* payload;
    };

    struct ShaderInitConfig
    {
        std::string name;
    };

    struct ShaderEntry
    {
        Shader* shader;
        std::string entryName;
    };

    class Shader : public RHIObject
    {
    public:
        Shader(const ShaderInitConfig& inInitConfig, void* inPayload) : initConfig(inInitConfig) { setPayload(inPayload); };
        ShaderEntry operator[](const char* name) { return ShaderEntry{ this,std::string(name) }; };
    public:
        const ShaderInitConfig initConfig;
    };

}