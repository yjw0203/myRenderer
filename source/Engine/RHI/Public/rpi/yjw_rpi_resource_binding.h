#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi_define.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_resource.h"
#include <memory>

namespace rpi
{
    enum RPIResourceSetType
    {
        vs = 0,
        ps = 1,
        cs = 2,
        common = 3,
        entity = 4
    };

    bool RPICheckResourceSetTypeID(int set_id, RPIResourceSetType type);
    int RPIGetResourceSetIDByType(RPIResourceSetType type);

    class RPIBuffer;
    class RPITexture;
    class RPIResourceSet
    {
    public:
        static RPIResourceSet Null;
        RPIResourceSet();
        RPIResourceSet(RHIResourceSet* rhiResourceSet);
        void SetBuffer(RHIName name, RPIBuffer buffer);
        void SetTexture(RHIName name, RPITexture texture);
        RHIResourceSet* GetRHIResourceSet();
        void Retain();
        void Release();
        bool IsNull();
        operator bool() { return !IsNull(); }
    private:
        RHIResourceSet* m_resource_set = nullptr;
    };

    class RPIPrimitiveBinding
    {
    public:
        static RPIPrimitiveBinding Null;
        RPIPrimitiveBinding();
        RPIPrimitiveBinding(RHIPrimitiveBinding* rhiPrimitiveBinding);
        void SetVertexBuffer(RHIName name, RPIBuffer buffer);
        int AddIndexBuffer(RPIBuffer buffer, int index_start, int index_count, bool is_index_16bit);
        RHIPrimitiveBinding* GetRHIPrimitiveBinding();
        RPIShader GetVertexShader();
        void Release();
        void Retain();
        bool IsNull();
        operator bool() { return !IsNull(); }
    private:
        RHIPrimitiveBinding* m_primitive_binding = nullptr;
    };

    struct RPIRenderPipelineDescriptor
    {
        RPIRenderPipelineDescriptor()
        {
            primitiveTopology = RHIPrimitiveTopology::primitive_topology_triangle_list;
        };
        RHIColorBlendState color_blend_state[RHI_MAX_RENDER_TARGETS] = {};
        RHIDepthStencilState depth_stencil_state{};
        RHIRasterizationState rasterization_state{};
        RHIPrimitiveTopology primitiveTopology{};
    };

    class RPIRenderPipeline
    {
    public:
        RPIRenderPipeline() {}
        RPIRenderPipeline(const RPIRenderPipelineDescriptor& descriptor);
        RHIRenderPipeline* GetRHIPipeline(RPIShader vs, RPIShader ps);

        struct PipelineWrapper
        {
            operator RHIRenderPipeline* () { return m_pipeline; }
            PipelineWrapper() {};
            PipelineWrapper(RHIRenderPipeline* pipeline)
            {
                m_pipeline = pipeline;
                m_pipeline->retain(nullptr);
            }
            ~PipelineWrapper()
            {
                m_pipeline->release();
            }
            RHIRenderPipeline* m_pipeline{};
        };
    private:
        std::shared_ptr<RHIRenderPipelineDescriptor> m_pipeline_descriptor;
        std::shared_ptr<std::unordered_map<uint64_t, std::shared_ptr<PipelineWrapper>>> m_pipelines;
    };
}