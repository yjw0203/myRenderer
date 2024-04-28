#include "Public/RHI/rpi/yjw_rpi_resource_binding.h"

namespace rpi
{
    RPIResourceBinding RPIResourceBinding::Null = RPIResourceBinding(nullptr);

    RPIResourceBinding::RPIResourceBinding()
    {
        m_resource_binding = nullptr;
    }

    RPIResourceBinding::RPIResourceBinding(RHIResourceBinding* rhiResourceBinding)
    {
        m_resource_binding = rhiResourceBinding;
    }

    void RPIResourceBinding::SetBuffer(RPIShaderType shaderType, RHIName name, RPIBuffer buffer)
    {
        m_resource_binding->SetBufferView(shaderType, name, buffer.GetView());
    }

    void RPIResourceBinding::SetTexture(RHIShaderType shaderType, RHIName name, RPITexture texture)
    {
        m_resource_binding->SetTextureView(shaderType, name, texture.GetView());
    }

    void RPIResourceBinding::SetVertexBuffer(RHIName name, RPIBuffer buffer)
    {
        m_resource_binding->SetVertexBuffer(name, buffer.GetBuffer());
    }

    void RPIResourceBinding::SetIndexBuffer(RPIBuffer buffer)
    {
        m_resource_binding->SetIndexBuffer(buffer.GetBuffer());
    }

    RHIResourceBinding* RPIResourceBinding::GetRHIResourceBinding()
    {
        return m_resource_binding;
    }

    void RPIResourceBinding::Release()
    {
        if (m_resource_binding)
        {
            m_resource_binding->release();
            m_resource_binding = nullptr;
        }
    }

    bool RPIResourceBinding::IsNull()
    {
        return m_resource_binding == nullptr;
    }
}