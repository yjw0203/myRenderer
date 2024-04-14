#pragma once

#include "ShaderConductor/ShaderConductor.hpp"

namespace rhi
{
    struct ShaderBlob
    {
        friend class ShaderCompiler;
    public:
        const void* Data();
        int Size();
        void Release();
    private:
        void SetBlob(ShaderConductor::Blob* blob);
        ShaderConductor::Blob* m_blob = nullptr;
    };

    class ShaderCompiler
    {
    public:
        ShaderBlob CompileFromFileHLSLToSpirv(ShaderConductor::ShaderStage shaderType, const char* url, const char* entryName);
        ShaderBlob CompileFromCodeHLSLToSpirv(ShaderConductor::ShaderStage shaderType, const char* code, const char* entryName);
    private:
        void ReadCodeFromFileUrl(const char* url, std::vector<char>& code);
    };
}