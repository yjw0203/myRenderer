#pragma once

#include "ShaderConductor/ShaderConductor.hpp"
#include "Engine/RHI/Public/rhi/yjw_rhi_define.h"

namespace rhi
{
    struct ShaderReflect
    {
        enum class ImageType
        {
            texture2D,
            textureCube,
            texture3D,
            texture2DArray
        };

        enum class DataType
        {
            float_,
            vec2,
            vec3,
            vec4,
            mat4,
            ivec4,
            int_
        };

        struct UBOMember
        {
            std::string m_name;
            DataType m_type;
            int m_offset;
        };

        struct UBO
        {
            std::string m_name;
            int m_block_size;
            int m_set;
            int m_binding;
            std::vector<UBOMember> m_members;
        };

        struct SeparateImage
        {
            std::string m_name;
            ImageType m_type;
            int m_set;
            int m_binding;
        };

        struct SamplerBuffer
        {
            std::string m_name;
            int m_set;
            int m_binding;
        };

        struct Input
        {
            DataType m_type;
            std::string m_name;
            int m_loacation;
        };

        struct Output
        {
            DataType m_type;
            std::string m_name;
            int m_loacation;
        };

        RHIShaderType m_shader_type{};
        std::string m_function_name{};
        std::vector<UBO> m_ubos;
        std::vector<SeparateImage> m_separate_images;
        std::vector<SamplerBuffer> m_sampler_buffers;
        std::vector<Input> m_inputs;
        std::vector<Output> m_outputs;

        static DataType ToDataType(const char* str);
        static RHIShaderType ToShaderType(const char* str);
        static ImageType ToImageType(const char* str);
        static int DataTypeToSize(ShaderReflect::DataType type);

        int GetVertexBindingCount() { return m_inputs.size(); }
        int GetVertexInputLocation(const std::string& name)
        {
            for (Input& input : m_inputs)
            {
                if (name == input.m_name)
                {
                    return input.m_loacation;
                }
            }
            return -1;
        }
    };

    struct ShaderBlob
    {
        friend class ShaderCompiler;
    public:
        const void* Data();
        int Size();
        void Release();
        void SetShaderReflect(const ShaderReflect& reflect);
        const ShaderReflect& GetReflect();
    private:
        void SetBlob(ShaderConductor::Blob* blob);
        void SetReflect(const ShaderReflect& reflect);
        ShaderReflect m_reflect{};
        ShaderConductor::Blob* m_blob = nullptr;
    };
    
    class ShaderCompiler
    {
    public:
        ShaderBlob CompileFromFileHLSLToSpirv(ShaderConductor::ShaderStage shaderType, const char* url, const char* entryName);
        ShaderBlob CompileFromCodeHLSLToSpirv(ShaderConductor::ShaderStage shaderType, const char* code, const char* entryName);
        ShaderReflect GetReflectFromSpirv(const void* compiledData, int compiledDataSize);
    private:
        void ReadCodeFromFileUrl(const char* url, std::vector<char>& code);
        void SaveToFile(const char* fileNmae, const void* data, int size);
    };
}