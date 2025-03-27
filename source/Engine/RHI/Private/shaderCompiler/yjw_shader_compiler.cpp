#include "Engine/RHI/Public/shaderCompiler/yjw_shader_compiler.h"

#include <fstream>
#include <iostream>

#define SHADER_CONDUCTOR_SOURCE
#include "ShaderConductor/ShaderConductor.hpp"
#include "spirv_cross/spirv_reflect.hpp"
#include "rapidjson/document.h"
#include "projectInfo.h"

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

    ShaderReflect::DataType ShaderReflect::ToDataType(const char* str)
    {
        if (strcmp(str, "float") == 0) {
            return ShaderReflect::DataType::float_;
        }
        else if (strcmp(str, "vec2") == 0){
            return ShaderReflect::DataType::vec2;
        }
        else if (strcmp(str, "vec3") == 0) {
            return ShaderReflect::DataType::vec3;
        }
        else if (strcmp(str, "vec4") == 0) {
            return ShaderReflect::DataType::vec4;
        }
        else if (strcmp(str, "mat4") == 0) {
            return ShaderReflect::DataType::mat4;
        }
        else if (strcmp(str, "ivec4") == 0) {
            return ShaderReflect::DataType::ivec4;
        }
        else if (strcmp(str, "int") == 0) {
            return ShaderReflect::DataType::int_;
        }
        assert(0);
        return (ShaderReflect::DataType)-1;
    }

    int ShaderReflect::DataTypeToSize(ShaderReflect::DataType type)
    {
        switch (type)
        {
        case ShaderReflect::DataType::float_:return 4;
        case ShaderReflect::DataType::vec2:return 8;
        case ShaderReflect::DataType::vec3:return 12;
        case ShaderReflect::DataType::vec4:return 16;
        case ShaderReflect::DataType::mat4:return 64;
        case ShaderReflect::DataType::ivec4:return 16;
        case ShaderReflect::DataType::int_:return 4;
        }
        assert(0);
        return -1;
    }

    RHIShaderType ShaderReflect::ToShaderType(const char* str)
    {
        if (strcmp(str, "vert") == 0) {
            return RHIShaderType::vertex;
        }
        else if (strcmp(str, "frag") == 0) {
            return RHIShaderType::fragment;
        }
        else if (strcmp(str, "compute") == 0) {
            return RHIShaderType::compute;
        }
        assert(0);
        return (RHIShaderType)-1;
    }

    ShaderReflect::ImageType ShaderReflect::ToImageType(const char* str)
    {
        if (strcmp(str, "texture2D") == 0){
            return ShaderReflect::ImageType::texture2D;
        }
        if (strcmp(str, "textureCube") == 0) {
            return ShaderReflect::ImageType::textureCube;
        }
        assert(0);
        return (ShaderReflect::ImageType)-1;
    }

    void ShaderBlob::SetShaderReflect(const ShaderReflect& reflect)
    {
        m_reflect = reflect;
    }

    const ShaderReflect& ShaderBlob::GetReflect()
    {
        return m_reflect;
    }

    ShaderConductor::Blob* ShaderBlob::GetBlob()
    {
        return m_blob;
    }

    void ShaderBlob::SetBlob(ShaderConductor::Blob* blob)
    {
        m_blob = blob;
    }

    void ShaderBlob::SetReflect(const ShaderReflect& reflect)
    {
        m_reflect = reflect;
    }

    ShaderBlob ShaderCompiler::CompileFromFileHLSLToSpirv(ShaderConductor::ShaderStage shaderType, const char* url, const char* entryName)
    {
        std::vector<char> code;
        ReadCodeFromFileUrl(url, code);
        code.push_back('\0');
        ShaderBlob blob = CompileFromCodeHLSLToSpirv(shaderType, code.data(), entryName);
        blob.SetShaderReflect(GetReflectFromSpirv(blob.Data(), blob.Size()));
        return blob;
    }

    ShaderBlob ShaderCompiler::CompileFromCodeHLSLToSpirv(ShaderConductor::ShaderStage shaderType, const char* code, const char* entryName)
    {
        std::vector<Blob*> blob_to_delete;

        auto loadIncludeCallback = [&](const char* includeName)
        {
            std::string url = std::string("") + SHADER_PATH + "/" + includeName;
            std::vector<char> code;
            ReadCodeFromFileUrl(url.c_str(), code);
            code.push_back('\0');
            Blob* blob = CreateBlob(code.data(), code.size());
            blob_to_delete.push_back(blob);
            return blob;
        };

        Compiler compiler;
        Compiler::SourceDesc sourceDesc{};
        sourceDesc.source = code;
        sourceDesc.entryPoint = entryName;
        sourceDesc.stage = shaderType;
        sourceDesc.defines = nullptr;
        sourceDesc.numDefines = 0;
        sourceDesc.loadIncludeCallback = loadIncludeCallback;

        Compiler::Options options{};
        options.packMatricesInRowMajor = true;
        options.enable16bitTypes = true;
        options.enableDebugInfo = true;
        options.disableOptimizations = true;
        options.optimizationLevel = 0;
        options.shaderModel = { 6, 2 };
        options.shiftAllCBuffersBindings = 0;
        options.shiftAllTexturesBindings = 16;
        options.shiftAllSamplersBindings = 32;
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

    ShaderReflect ShaderCompiler::GetReflectFromSpirv(const void* compiledData, int compiledDataSize)
    {
        ShaderReflect reflect{};
        spirv_cross::CompilerReflection reflection((uint32_t*)compiledData, compiledDataSize / sizeof(uint32_t));
        spirv_cross::ShaderResources resources = reflection.get_shader_resources();
        std::string reflect_json_str = reflection.compile();
        rapidjson::Document document;
        document.Parse(reflect_json_str.c_str());
        assert(document.IsObject());
        assert(document.HasMember("entryPoints"));
        auto& entryPoints = document["entryPoints"][0];
        assert(entryPoints.HasMember("name"));
        assert(entryPoints.HasMember("mode"));

        reflect.m_function_name = entryPoints["name"].GetString();
        reflect.m_shader_type = ShaderReflect::ToShaderType(entryPoints["mode"].GetString());

        if (document.HasMember("separate_images"))
        {
            auto& separate_images = document["separate_images"];
            for (int index = 0; index < separate_images.Size(); index++)
            {
                auto& image_json = separate_images[index];
                assert(image_json.IsObject());
                std::string type_name = image_json["type"].GetString();
                if (type_name == "samplerBuffer")
                {
                    ShaderReflect::SamplerBuffer buffer{};
                    buffer.m_name = image_json["name"].GetString();
                    buffer.m_set = image_json["set"].GetInt();
                    buffer.m_binding = image_json["binding"].GetInt();
                    reflect.m_sampler_buffers.push_back(buffer);
                }
                else
                {
                    ShaderReflect::SeparateImage image{};
                    image.m_name = image_json["name"].GetString();
                    image.m_set = image_json["set"].GetInt();
                    image.m_binding = image_json["binding"].GetInt();
                    image.m_type = ShaderReflect::ToImageType(type_name.c_str());
                    reflect.m_separate_images.push_back(image);
                }
            }
        }

        if (document.HasMember("ubos"))
        {
            auto& ubos = document["ubos"];
            auto& types = document["types"];
            for (int index = 0; index < ubos.Size(); index++)
            {
                auto& ubo_json = ubos[index];
                assert(ubo_json.IsObject());
                ShaderReflect::UBO ubo{};
                ubo.m_name = ubo_json["name"].GetString() + 5;//add 5 to erase prefix "type."
                ubo.m_block_size = ubo_json["block_size"].GetInt();
                ubo.m_set = ubo_json["set"].GetInt();
                ubo.m_binding = ubo_json["binding"].GetInt();
                std::string type_name = ubo_json["type"].GetString();
                if (types.HasMember(type_name.c_str()))
                {
                    auto& type = types[type_name.c_str()]["members"];
                    for (int index = 0; index < type.Size(); index++)
                    {
                        auto& member_json = type[index];
                        ShaderReflect::UBOMember member;
                        member.m_name = member_json["name"].GetString();
                        member.m_offset = member_json["offset"].GetInt();
                        member.m_type = ShaderReflect::ToDataType(member_json["type"].GetString());
                        ubo.m_members.push_back(member);
                    }
                }
                reflect.m_ubos.push_back(ubo);
            }
        }

        if (document.HasMember("inputs"))
        {
            auto& inputs = document["inputs"];
            for (int index = 0; index < inputs.Size(); index++)
            {
                auto& input_json = inputs[index];
                assert(input_json.IsObject());
                ShaderReflect::Input input{};
                input.m_name = input_json["name"].GetString() + 7;//add 7 to erase prefix "in.var."
                input.m_type = ShaderReflect::ToDataType(input_json["type"].GetString());
                input.m_loacation = input_json["location"].GetInt();
                reflect.m_inputs.push_back(input);
            }
        }

        if (document.HasMember("outputs"))
        {
            auto& outputs = document["outputs"];
            for (int index = 0; index < outputs.Size(); index++)
            {
                auto& output_json = outputs[index];
                assert(output_json.IsObject());
                ShaderReflect::Output output{};
                output.m_name = output_json["name"].GetString() + 8;//add 8 to erase prefix "out.var."
                output.m_type = ShaderReflect::ToDataType(output_json["type"].GetString());
                output.m_loacation = output_json["location"].GetInt();
                reflect.m_outputs.push_back(output);
            }
        }

        return std::move(reflect);
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

    void ShaderCompiler::SaveToFile(const char* fileName, const void* data, int size)
    {
        std::ofstream file(fileName, std::ios::out | std::ios::binary);
        if (file.is_open()) {
            file.write(static_cast<const char*>(data), size);
            file.close();
        }
    }
}