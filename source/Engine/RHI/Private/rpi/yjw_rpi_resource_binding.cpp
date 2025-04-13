#include "Engine/RHI/Public/rpi/yjw_rpi_resource_binding.h"
#include "Engine/RHI/Public/rpi/yjw_rpi.h"

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

    void RPIResourceSet::Retain()
    {
        if (m_resource_set)
        {
            m_resource_set->retain(nullptr);
        }
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

    RPIShader RPIPrimitiveBinding::GetVertexShader()
    {
        return m_primitive_binding->GetVertexShader();
    }

    void RPIPrimitiveBinding::Release()
    {
        if (m_primitive_binding)
        {
            int ref = m_primitive_binding->release();
            if (ref == 0)
            {
                m_primitive_binding = nullptr;
            }
        }
    }

    void RPIPrimitiveBinding::Retain()
    {
        if (m_primitive_binding)
        {
            m_primitive_binding->retain(nullptr);
        }
    }

    bool RPIPrimitiveBinding::IsNull()
    {
        return m_primitive_binding == nullptr;
    }

    RPIRenderPipeline::RPIRenderPipeline(const RPIRenderPipelineDescriptor& descriptor)
    {
        m_pipelines = std::make_shared<std::unordered_map<uint64_t, std::shared_ptr<PipelineWrapper>>>();
        m_pipeline_descriptor = std::make_shared<RHIRenderPipelineDescriptor>();
        for (int i = 0; i < RHI_MAX_RENDER_TARGETS; i++)
        {
            m_pipeline_descriptor->color_blend_state[i] = descriptor.color_blend_state[i];
        }
        m_pipeline_descriptor->depth_stencil_state = descriptor.depth_stencil_state;
        m_pipeline_descriptor->rasterization_state = descriptor.rasterization_state;
        m_pipeline_descriptor->primitiveTopology = descriptor.primitiveTopology;
    }

    RHIRenderPipeline* RPIRenderPipeline::GetRHIPipeline(RPIShader vs, RPIShader ps)
    {
        uint64_t hash = (uint64_t)vs * 233333 + (uint64_t)ps;
        if (m_pipelines->find(hash) != m_pipelines->end())
        {
            return *(*m_pipelines)[hash];
        }
        m_pipeline_descriptor->vs = vs;
        m_pipeline_descriptor->ps = ps;
        (*m_pipelines)[hash] = std::make_shared<PipelineWrapper>(RPIO(device)->CreateRenderPipeline(*m_pipeline_descriptor));
        return *(*m_pipelines)[hash];
    }
}