#include "yjw_shader_compiler.h"

#include <fstream>
#include <iostream>

#define SHADER_CONDUCTOR_SOURCE
#include "ShaderConductor/ShaderConductor.hpp"

#include <cassert>

namespace rhi
{
    using namespace ShaderConductor;
    const void* ShaderBlob::Data()
    {
        if (m_blob)
        {
            return m_blob->Data();
        }
        return nullptr;
    }

    int ShaderBlob::Size()
    {
        if (m_blob)
        {
            return m_blob->Size();
        }
        return 0;
    }

    void ShaderBlob::Release()
    {
        if (m_blob)
        {
            DestroyBlob(m_blob);
        }
    }

    void ShaderBlob::SetBlob(ShaderConductor::Blob* blob)
    {
        m_blob = blob;
    }

    ShaderBlob ShaderCompiler::CompileFromFileHLSLToSpirv(ShaderConductor::ShaderStage shaderType, const char* url, const char* entryName)
    {
        std::vector<char> code;
        ReadCodeFromFileUrl(url, code);
        code.push_back('\0');
        return CompileFromCodeHLSLToSpirv(shaderType, code.data(), entryName);
    }

    ShaderBlob ShaderCompiler::CompileFromCodeHLSLToSpirv(ShaderConductor::ShaderStage shaderType, const char* code, const char* entryName)
    {
        Compiler compiler;
        Compiler::SourceDesc sourceDesc{};
        sourceDesc.source = code;
        sourceDesc.entryPoint = entryName;
        sourceDesc.stage = shaderType;
        sourceDesc.defines = nullptr;
        sourceDesc.numDefines = 0;

        Compiler::Options options{};
        options.packMatricesInRowMajor = true;
        options.enable16bitTypes = true;
        options.enableDebugInfo = true;
        options.disableOptimizations = false;
        options.optimizationLevel = 0;
        options.shaderModel = { 6, 2 };
        options.shiftAllTexturesBindings = 0;
        options.shiftAllSamplersBindings = 16;
        options.shiftAllCBuffersBindings = 32;
        options.shiftAllUABuffersBindings = 48;

        Compiler::TargetDesc targetDesc{};
        targetDesc.language = ShadingLanguage::SpirV;
        targetDesc.version = "";
        targetDesc.asModule = true;

        Compiler::ResultDesc result = compiler.Compile(sourceDesc, options, targetDesc);
        if (result.errorWarningMsg)
        {
            std::vector<char> errorMsg;
            errorMsg.resize(result.errorWarningMsg->Size() + 1);
            memcpy(errorMsg.data(), result.errorWarningMsg->Data(), result.errorWarningMsg->Size());
            errorMsg.back() = '\0';
            std::cerr << "Shader conversion failed: " << errorMsg.data() << std::endl;
            DestroyBlob(result.errorWarningMsg);
        }
        if (result.hasError)
        {
            assert(0);
        }
        if (result.target)
        {
            ShaderBlob blob{};
            blob.SetBlob(result.target);
            return blob;
        }
        else
        {
            return ShaderBlob{};
        }
    }

    void ShaderCompiler::ReadCodeFromFileUrl(const char* url, std::vector<char>& code)
    {
        std::string filename(url);
        std::ifstream file(filename, std::ios::binary);
        if (!file)
        {
            std::cout << "无法打开文件：" << filename << std::endl;
            return;
        }
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        code.resize(fileSize);
        file.read(code.data(), fileSize);
        file.close();
    }
}