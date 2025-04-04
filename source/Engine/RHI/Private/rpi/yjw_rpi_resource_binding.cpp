#include "Engine/RHI/Public/rpi/yjw_rpi_resource_binding.h"

namespace rpi
{
    bool RPICheckResourceSetTypeID(int set_id, RPIResourceSetType type)
    {
        return set_id == (int)type;
    }

    int RPIGetResourceSetIDByType(RPIResourceSetType type)
    {
        return (int)type;
    }

    RPIResourceSet RPIResourceSet::Null = RPIResourceSet(nullptr);

    RPIResourceSet::RPIResourceSet()
    {
        m_resource_set = nullptr;
    }

    RPIResourceSet::RPIResourceSet(RHIResourceSet* rhiResourceSet)
    {
        m_resource_set = rhiResourceSet;
    }

    void RPIResourceSet::SetBuffer(RHIName name, RPIBuffer buffer)
    {
        m_resource_set->SetBufferView(name, buffer.GetView());
    }

    void RPIResourceSet::SetTexture(RHIName name, RPITexture texture)
    {
        m_resource_set->SetTextureView(name, texture.GetView());
    }

    RHIResourceSet* RPIResourceSet::GetRHIResourceSet()
    {
        return m_resource_set;
    }

    void RPIResourceSet::Release()
    {
        if (m_resource_set)
        {
            m_resource_set->release();
            m_resource_set = nullptr;
        }
    }

    bool RPIResourceSet::IsNull()
    {
        return m_resource_set == nullptr;
    }

    RPIResourceBinding::RPIResourceBinding()
    {}
    RPIResourceBinding::RPIResourceBinding(RHIResourceBinding* rhiResourceBinding)
    {
        m_resource_binding = rhiResourceBinding;
    }
    void RPIResourceBinding::SetResourceSet(RPIResourceSetType type, RPIResourceSet set)
    {
        if (m_resource_binding)
        {
            m_resource_binding->SetResourceSet((int)type, set.GetRHIResourceSet());
        }
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

    int RPIPrimitiveBinding::AddIndexBuffer(RPIBuffer buffer, int index_start, int index_count, bool is_index_16bit)
    {
        return m_primitive_binding->AddIndexBuffer(buffer.GetView(),index_start, index_count, is_index_16bit);
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