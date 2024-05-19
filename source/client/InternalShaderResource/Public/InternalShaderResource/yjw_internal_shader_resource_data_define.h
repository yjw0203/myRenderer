#pragma once

#include <glm/glm.hpp>
#include <string>

namespace yjw
{
    namespace InternalShaderData
    {
        struct alignas(64) Camera
        {
            glm::mat4x4 viewMat;
            glm::mat4x4 projectMat;
            alignas(16) glm::vec3 cameraPos;
        };

        struct alignas(64) Light
        {
            alignas(16) glm::vec3 lightPos;
            alignas(16) glm::vec3 lightColor;
        };

        struct alignas(64) Option
        {
            alignas(16) glm::vec2 screenSize;
        };
    }

    /* 
    template<class T>
    struct TInternalShaderInfo
    {
    };
    template<>
    struct TInternalShaderInfo<InternalShaderData::Camera>
    {
        inline static const std::string shaderName = "camera";
    };
    template<>
    struct TInternalShaderInfo<InternalShaderData::Light>
    {
        inline static const std::string shaderName = "light";
    };
    template<>
    struct TInternalShaderInfo<InternalShaderData::Option>
    {
        inline static const std::string shaderName = "option";
    };*/
}