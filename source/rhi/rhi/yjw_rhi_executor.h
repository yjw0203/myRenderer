#pragma once
#include "yjw_rhi_pipeline.h"
#include "yjw_rhi_descriptr_set.h"

namespace rhi
{
    class RHIExecutor
    {
    public:
        RHIExecutor(std::shared_ptr<RHIPipeline> pipeline, std::shared_ptr<RHIAttachmentsSet> attachmentsSet);
        void beginPass();
        void endPass();
        void bindDescriptorSet(const RHIDescriptorsSet* descriptorsSet);
        void bindVertexBuffer(RHIResource* vertex_buffer);
        void bindIndexBuffer(RHIResource* index_buffer);
        void draw(uint32_t vertexCount,  uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
        void drawIndex(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
    private:
        std::shared_ptr<RHIPipeline> pipeline;
        std::shared_ptr<RHIAttachmentsSet> attachmentsSet;
    };
}