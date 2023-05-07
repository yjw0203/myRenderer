#pragma once
#include <vector>
#include <string>
namespace rhi
{
    enum BaseDataType
    {
        None,
        Boolean,
        Int,
        Half,
        Float,
        Double,
        IntArray,
        FloatArray,
        Vector2,
        Vector3,
        Vector4,
        Matrix3,
        Matrix4
    };

    struct BlockBufferMember
    {
        BaseDataType type;
        std::string name;
        uint32_t size;
        uint32_t offset;
    };

    struct BlockBuffer
    {
        std::string name;
        std::vector<BlockBufferMember> members;
        uint32_t size;
        uint32_t set;
        uint32_t binding;
    };

    struct Sampler2D
    {
        std::string name;
        uint32_t set;
        uint32_t binding;
    };

    struct VulkanShaderReflectionData
    {
        std::vector<Sampler2D> sampler2Ds;
        std::vector<BlockBuffer> blockBuffers;
        uint32_t setCount;
    };
 
    class VulkanShaderReflecter
    {
    public:
        static void reflect(std::vector<uint32_t>& code, VulkanShaderReflectionData& out_data);
    };
}