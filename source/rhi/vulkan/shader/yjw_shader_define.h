#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "vulkan/vulkan.h"
#include "rhi/vulkan/yjw_vulkan_global.h"

namespace vulkan {
	class Shader
	{
	public:
        std::unordered_map<std::string, struct ShaderReflect>reflects;
        VkShaderModule shaderModule{};
	};

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

	struct ShaderReflect
	{
        VkShaderStageFlagBits stage;
        std::vector<Sampler2D> sampler2Ds;
        std::vector<BlockBuffer> blockBuffers;
        uint32_t setCount;
	};

}