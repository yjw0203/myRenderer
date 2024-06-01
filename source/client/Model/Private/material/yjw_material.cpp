#include "Public/Model/yjw_material.h"

namespace yjw
{
    void MaterialParameterPool::AddParameter(const std::string& name, int width)
    {

    }

    void Material::BuildPipeline()
    {
        using namespace rpi;
        RPIShader vs = RPICreateShader(RPIShaderType::vertex, m_vs.shader_path.c_str(), m_vs.entry_name.c_str());
        RPIShader ps = RPICreateShader(RPIShaderType::fragment, m_ps.shader_path.c_str(), m_ps.entry_name.c_str());
        RPIRenderPipelineDescriptor pipelineDesc = RPIGetDefaultRenderPipeline();
        pipelineDesc.vs = vs;
        pipelineDesc.ps = ps;
        pipelineDesc.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_depth_read_and_write);
        m_pipeline = RPICreateRenderPipeline(pipelineDesc);
        m_resource_binding = RPICreateResourceBinding(m_pipeline);
        vs->release();
        ps->release();
    }

    rpi::RPIPipeline Material::GetPipeline()
    {
        return m_pipeline;
    }

    MaterialInstance::MaterialInstance(Material* material)
    {
        using namespace rpi;
        m_resource_binding = RPICreateResourceBinding(material->GetPipeline());
    }
}