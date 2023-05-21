#include "yjw_rhi_executor.h"
#include "yjw_rhi_interface.h"

namespace rhi
{
    RHIExecutor::RHIExecutor(std::shared_ptr<RHIPipeline> pipeline, std::shared_ptr<RHIAttachmentsSet> attachmentsSet)
    {
        this->pipeline = pipeline;
        this->attachmentsSet = attachmentsSet;
    }

    void RHIExecutor::beginPass()
    {
        IRHI::Get()->beginPass(pipeline, attachmentsSet);
    }

    void RHIExecutor::endPass()
    {
        IRHI::Get()->endPass();
    }

    void RHIExecutor::bindDescriptorSet(const RHIDescriptorsSet* descriptorsSet)
    {
        IRHI::Get()->bindDescriptorSet(descriptorsSet);
    }

    void RHIExecutor::bindVertexBuffer(RHIResource* vertex_buffer)
    {
        IRHI::Get()->bindVertexBuffer(vertex_buffer);
    }

    void RHIExecutor::bindIndexBuffer(RHIResource* index_buffer)
    {
        IRHI::Get()->bindIndexBuffer(index_buffer);
    }

    void RHIExecutor::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
    {
        IRHI::Get()->draw(vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void RHIExecutor::drawIndex(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
    {
        IRHI::Get()->drawIndex(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }
}