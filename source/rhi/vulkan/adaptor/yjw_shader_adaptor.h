#pragma once
#include "rhi/new_rhi/yjw_rhi_shader.h"
#include "rhi/vulkan/shader/yjw_shader_utils.h"
namespace vulkan
{
    class ShaderAdaptor
    {
    public:
        ShaderAdaptor(const rhi::Shader& shader) : payload((Shader*)shader.payload) {}
        ShaderAdaptor(Shader& shader) : payload(&shader) {}
        operator rhi::ShaderPayload() { return rhi::ShaderPayload{ payload }; }
        operator Shader* () { return payload; }
    private:
        Shader* payload = nullptr;
    };
}