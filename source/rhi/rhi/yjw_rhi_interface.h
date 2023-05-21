#pragma once
#include <memory>
namespace rhi
{
    struct RHIResourceDesc;
    class RHIResourceLocation;
    class RHIResourceViewLocation;
    struct RHIShaderDesc;
    class RHIResource;
    class RHIShaderLocation;
    class DefaultDrawTemplate;
    class DefaultDrawTemplateBuildCache;
    enum class ResourceViewType;
    enum RHIFormat;
    enum RHIResourceState;
    class RHIResourceView;
    struct RHIPipelineLocation;
    class RHIPipeline;
    struct RHIAttachmentsSetLocation;
    class RHIAttachmentsSet;
    struct RHIDescriptorsSetLocation;
    class RHIDescriptorsSet;

    struct CreateInfo
    {
        void* window;
    };

    class IRHI
    {
    public:
        static IRHI* Get();

        virtual void initialize(CreateInfo info) = 0;

        virtual void beginFrame() = 0;
        virtual void endFrame(RHIResource* present_texture) = 0;


        virtual void resourceBarrier(RHIResource* resource, RHIResourceState beforeState, RHIResourceState afterState) = 0;
        
        virtual void resourceBarrierImmidiately(RHIResource* resource, RHIResourceState beforeState, RHIResourceState afterState) = 0;

        virtual void copyResourceImmidiately(RHIResource* src, RHIResource* dst) = 0;

        virtual void writeResourceImmidiately(RHIResource* resource, void* data, int size, int offset) = 0;

        virtual void clearImageResource(RHIResource* resource) = 0;


    //todo: will be move to unpublic.
        virtual void beginPass(std::shared_ptr<RHIPipeline> pipeline, std::shared_ptr<RHIAttachmentsSet> attachmentsSet) = 0;
        virtual void endPass() = 0;
        virtual void bindVertexBuffer(RHIResource* vertex_buffer) = 0;
        virtual void bindIndexBuffer(RHIResource* index_buffer) = 0;
        virtual void bindDescriptorSet(const RHIDescriptorsSet* descriptorsSet) = 0;
        virtual void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) = 0;
        virtual void drawIndex(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) = 0;

        virtual std::shared_ptr<RHIPipelineLocation> createPipeline(std::shared_ptr<RHIPipeline> pipeline) = 0;
        virtual void destoryPipeline(std::shared_ptr<RHIPipelineLocation> location) = 0;

        virtual std::shared_ptr<RHIAttachmentsSetLocation> createAttachmentsSet(std::shared_ptr<RHIAttachmentsSet> attachmentsSet) = 0;
        virtual void destoryAttachmentsSet(std::shared_ptr<RHIAttachmentsSetLocation> location) = 0;

        virtual std::shared_ptr<RHIDescriptorsSetLocation> createDescriptorsSet(std::shared_ptr<RHIDescriptorsSet> descriptorsSet) = 0;
        virtual void destoryDescriptorsSet(std::shared_ptr<RHIDescriptorsSetLocation> location) = 0;

        //resource
        virtual RHIResourceLocation* createResource(const RHIResourceDesc& desc) = 0;
        virtual void deleteResource(RHIResourceLocation*& location) = 0;

        //resource_view
        virtual RHIResourceViewLocation* createResourceView(ResourceViewType type, RHIResource* resource, RHIFormat view_format) = 0;
        virtual void deleteResourceView(RHIResourceViewLocation*& location) = 0;

        //shader
        virtual RHIShaderLocation* createShader(const RHIShaderDesc& desc) = 0;
        virtual void deleteShader(RHIShaderLocation*& location) = 0;

    };
}