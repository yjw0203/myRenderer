#include "Engine/Render/Private/Pass/PickPass.h"
#include "projectInfo.h"

namespace yjw
{
    void PickPass::Initialize()
    {
        m_ps = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(Pick.hlsl), "PSMain");
    }

    void PickPass::Destroy()
    {

    }

    void PickPass::Submit(RPIContext context)
    {

    }
}