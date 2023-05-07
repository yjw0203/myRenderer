#include "yjw_rhi_shader.h"
#include "yjw_rhi_interface.h"

namespace rhi
{
    RHIShader::RHIShader(std::string filePath)
        : rhiShaderDesc(filePath)
    {
        shaderLocation = IRHI::Get()->createShader(rhiShaderDesc);
    }

    RHIShader::~RHIShader()
    {
        IRHI::Get()->deleteShader(shaderLocation);
    }

    RHIShaderView::RHIShaderView(RHIShader* shader, RHIShaderType type, std::string entry)
        : shader(shader), type(type), entry(entry)
    {

    }

    RHIShader* RHIShaderView::getShader()
    {
        return shader;
    }
    RHIShaderType RHIShaderView::getType()
    {
        return type;
    }
    std::string& RHIShaderView::getEntry()
    {
        return entry;
    }
    RHIShaderViewData& RHIShaderView::getData()
    {
        return data;
    }
    void RHIShaderView::setDataTexture(std::string varName, RHIResourceView* view)
    {
        data.texture2Ds[varName] = view;
    }
    void RHIShaderView::setDataBuffer(std::string varName, RHIResourceView* view)
    {
        data.uniformBuffers[varName] = view;
    }
}