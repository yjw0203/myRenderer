#pragma once
#include <string>
#include <unordered_map>
#include "yjw_rhi_resource_view.h"

namespace rhi
{
    enum RHIShaderType
    {
        vertex_shader,
        pixel_shader,
        compute_shader
    };

    struct RHIShaderDesc
    {
        std::string filePath;
        RHIShaderDesc(std::string in_filePath) : filePath(in_filePath)
        {
        }
    };

    class RHIShaderLocation{};

    class RHIShader
    {
    public:
        RHIShader(std::string filePath);
        virtual ~RHIShader();
        const RHIShaderDesc rhiShaderDesc;
        RHIShaderLocation* shaderLocation = nullptr;
    };

    struct RHIShaderViewData
    {
        std::unordered_map<std::string, RHIResourceView*>texture2Ds;
        std::unordered_map<std::string, RHIResourceView*>uniformBuffers;
        //std::unordered_map<std::string, RHISampler*>sampler2Ds;
    };

    class RHIShaderView
    {
    public:
        RHIShaderView(RHIShader* shader, RHIShaderType type, std::string entry);

        RHIShader* getShader();
        RHIShaderType getType();
        std::string& getEntry();
        RHIShaderViewData& getData();

        void setDataTexture(std::string varName,RHIResourceView* view);
        void setDataBuffer(std::string varName,RHIResourceView* view);
    private:
        RHIShader* shader = nullptr;
        RHIShaderType type;
        std::string entry;
        RHIShaderViewData data;
    };

}