#pragma once
#include <vulkan/vulkan.h>
#include "rhi/rhi/yjw_rhi_header.h"

namespace rhi
{

    class IVulkanRHI : public IRHI
    {
    public:
        virtual void initialize(CreateInfo info) override;

        virtual void beginFrame() override;
        virtual void endFrame(RHIResource* present_texture) override;

        virtual void resourceBarrier(RHIResource* resource, RHIResourceState beforeState, RHIResourceState afterState) override;
        virtual void resourceBarrierImmidiately(RHIResource* resource, RHIResourceState beforeState, RHIResourceState afterState) override;

        virtual void copyResourceImmidiately(RHIResource* src, RHIResource* dst) override;

        virtual void writeResourceImmidiately(RHIResource* resource, void* data, int size, int offset) override;

        virtual void clearImageResource(RHIResource* resource) override;

        //executor
        virtual void beginPass(std::shared_ptr<RHIPipeline> pipeline, std::shared_ptr<RHIAttachmentsSet> attachmentsSet) override;
        virtual void endPass() override;
        virtual void bindVertexBuffer(RHIResource* vertex_buffer) override;
        virtual void bindIndexBuffer(RHIResource* index_buffer) override;
        virtual void bindDescriptorSet(const RHIDescriptorsSet* descriptorsSet) override;
        virtual void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
        virtual void drawIndex(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) override;

        //pipeline
        virtual std::shared_ptr<RHIPipelineLocation> createPipeline(std::shared_ptr<RHIPipeline> pipeline) override;
        virtual void destoryPipeline(std::shared_ptr<RHIPipelineLocation> location) override;

        virtual std::shared_ptr<RHIAttachmentsSetLocation> createAttachmentsSet(std::shared_ptr<RHIAttachmentsSet> attachmentsSet) override;
        virtual void destoryAttachmentsSet(std::shared_ptr<RHIAttachmentsSetLocation> location) override;

        virtual std::shared_ptr<RHIDescriptorsSetLocation> createDescriptorsSet(std::shared_ptr<RHIDescriptorsSet> descriptorsSet) override;
        virtual void destoryDescriptorsSet(std::shared_ptr<RHIDescriptorsSetLocation> location) override;

        //resource
        virtual RHIResourceLocation* createResource(const RHIResourceDesc& desc) override;
        virtual void deleteResource(RHIResourceLocation*& location) override;

        //resource_view
        virtual RHIResourceViewLocation* createResourceView(ResourceViewType type, RHIResource* resource, RHIFormat view_format) override;
        virtual void deleteResourceView(RHIResourceViewLocation*& location) override;

        //shader
        virtual RHIShaderLocation* createShader(const RHIShaderDesc& desc) override;
        virtual void deleteShader(RHIShaderLocation*& location) override;

    };
}