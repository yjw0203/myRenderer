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

    RPIPrimitiveBinding::RPIPrimitiveBinding()
    {
        m_primitive_binding = nullptr;
    }

    RPIPrimitiveBinding::RPIPrimitiveBinding(RHIPrimitiveBinding* rhiPrimitiveBinding)
    {
        m_primitive_binding = rhiPrimitiveBinding;
    }

    void RPIPrimitiveBinding::SetVertexBuffer(RHIName name, RPIBuffer buffer)
    {
        m_primitive_binding->SetVertexBuffer(name, buffer.GetView());
    }

    void RPIPrimitiveBinding::SetIndexBuffer(RPIBuffer buffer, int index_start, int index_count, bool is_index_16bit)
    {
        m_primitive_binding->SetIndexBuffer(buffer.GetView(),index_start, index_count, is_index_16bit);
    }

    RHIPrimitiveBinding* RPIPrimitiveBinding::GetRHIPrimitiveBinding()
    {
        return m_primitive_binding;
    }

    void RPIPrimitiveBinding::Release()
    {
        if (m_primitive_binding)
        {
            m_primitive_binding->release();
            m_primitive_binding = nullptr;
        }
    }

    bool RPIPrimitiveBinding::IsNull()
    {
        return m_primitive_binding == nullptr;
    }
}