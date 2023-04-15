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

    struct Sampler2D
    {
        std::string name;
        uint32_t set;
        uint32_t binding;
    };

    struct VulkanShaderReflectionData
    {
        std::vector<Sampler2D> sampler2Ds;
        uint32_t setCount;
    };
 
    class VulkanShaderReflecter
    {
    public:
        static void reflect(std::vector<uint32_t>& code, VulkanShaderReflectionData& out_data);
    };
}