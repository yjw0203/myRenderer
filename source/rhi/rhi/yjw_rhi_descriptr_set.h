#pragma once
#include "yjw_rhi_resource.h"
#include "yjw_rhi_resource_view.h"
#include "yjw_rhi_interface.h"
#include <unordered_map>
#include <memory>
namespace rhi
{
    class RHIColorAttachment
    {
    public:
        RHIColorAttachment(RHIResource* resource,RHIFormat format)
        {
            resourceView = new RHIResourceView(ResourceViewType::rtv, resource, format);
        }
        virtual ~RHIColorAttachment()
        {
            delete resourceView;
        }
        RHIResourceView* resourceView = nullptr;
    };

    class RHIDepthStencilAttachment
    {
    public:
        RHIDepthStencilAttachment(RHIResource* resource, RHIFormat format)
        {
            resourceView = new RHIResourceView(ResourceViewType::dsv, resource, format);
        }
        virtual ~RHIDepthStencilAttachment()
        {
            delete resourceView;
        }
        RHIResourceView* resourceView = nullptr;
    };

    enum RHIDescriptorType
    {
        uniform_buffer,
        shader_resource_texture
    };
    class RHIDescriptor 
    {
    public:
        RHIDescriptorType type;
    };
    
    class RHIUniformBuffer : public RHIDescriptor
    {
    public:
        RHIUniformBuffer(int size) :size(size)
        {
            type = RHIDescriptorType::uniform_buffer;
            resource = new RHIBuffer(size, RHIResourceUsageBits::none, RHIMemoryType::upload);
        }
        void update(void* data, int size, int offset)
        {
            IRHI::Get()->writeResourceImmidiately(resource, data, size, offset);
        }
        const int size;
        RHIResource* resource = nullptr;
        virtual ~RHIUniformBuffer()
        {
            delete resource;
        }
    };

    class RHIShaderResourceTexture : public RHIDescriptor
    {
    public:
        RHIShaderResourceTexture(RHIResource* resource, RHIFormat format)
        {
            type = RHIDescriptorType::shader_resource_texture;
            resourceView = new RHIResourceView(ResourceViewType::srv, resource, format);
        }
        RHIResourceView* resourceView = nullptr;
        virtual ~RHIShaderResourceTexture()
        {
            delete resourceView;
        }
    };

    struct RHIAttachmentsSetLocation {};
    class RHIAttachmentsSetCreator;
    class RHIAttachmentsSet
    {
    public:
        static RHIAttachmentsSetCreator New(std::shared_ptr<RHIPipeline> pipeline, int width, int height);
        ~RHIAttachmentsSet();
        std::unordered_map<int, std::shared_ptr<RHIColorAttachment> > color_attachments;
        std::shared_ptr<RHIDepthStencilAttachment> depth_stencil_attachments;
        std::shared_ptr<RHIAttachmentsSetLocation> location;
        std::shared_ptr<RHIPipeline> pipeline;
        int width;
        int height;
    };
    class RHIAttachmentsSetCreator
    {
        friend class RHIAttachmentsSet;
    public:
        RHIAttachmentsSetCreator(std::shared_ptr<RHIPipeline> pipeline, int width, int height)
        {
            attachmentSet = std::make_shared<RHIAttachmentsSet>();
            attachmentSet->pipeline = pipeline;
            attachmentSet->width = width;
            attachmentSet->height = height;
        }

        operator std::shared_ptr<RHIAttachmentsSet>() const
        {
            attachmentSet->location = IRHI::Get()->createAttachmentsSet(attachmentSet);
            return std::move(attachmentSet);
        }
        template<int slot>
        RHIAttachmentsSetCreator& COLOR_ATTACHMENT(std::shared_ptr<RHIColorAttachment> attachment) { attachmentSet->color_attachments[slot] = attachment; return *this; }
        RHIAttachmentsSetCreator& DEPTH_STENCIL_ATTACHMENT(std::shared_ptr<RHIDepthStencilAttachment> attachment) { attachmentSet->depth_stencil_attachments = attachment; return *this; }
    private:
        std::shared_ptr<RHIAttachmentsSet> attachmentSet;
    };
    class RHIAttachmentSetDestoryer
    {
        friend class RHIAttachmentsSet;
    public:
        RHIAttachmentSetDestoryer(RHIAttachmentsSet* attachmentSet) { this->attachmentSet = attachmentSet; }
        virtual ~RHIAttachmentSetDestoryer(){ IRHI::Get()->destoryAttachmentsSet(attachmentSet->location); }
    private:
        RHIAttachmentsSet* attachmentSet;
    };

    struct RHIDescriptorsSetLocation {};
    class RHIDescriptorsSetCreator;
    class RHIDescriptorsSet
    {
        struct SimplePairHash {
            std::uint64_t operator()(const std::pair<int, int>& p) const {
                return ((uint64_t)p.first << 32) + p.second;
            }
        };
    public:
        static RHIDescriptorsSetCreator New(std::shared_ptr<RHIPipeline> pipeline);
        ~RHIDescriptorsSet();
        int num_set = 0;
        std::unordered_map<std::pair<int, int>,std::shared_ptr<RHIDescriptor>, SimplePairHash > decriptors;
        std::shared_ptr<RHIDescriptorsSetLocation> location;
        std::shared_ptr<RHIPipeline> pipeline;
    };
    class RHIDescriptorsSetCreator
    {
        friend class RHIDescriptorsSet;
    public:
        RHIDescriptorsSetCreator(std::shared_ptr<RHIPipeline> pipeline)
        {
            descriptorsSet = std::make_shared<RHIDescriptorsSet>();
            descriptorsSet->pipeline = pipeline;
        }
        operator std::shared_ptr<RHIDescriptorsSet>() const
        {
            descriptorsSet->location = IRHI::Get()->createDescriptorsSet(descriptorsSet);
            return std::move(descriptorsSet);
        }
        template<int set,int binding>
        RHIDescriptorsSetCreator& DESCRIPTOR(std::shared_ptr<RHIDescriptor> descriptor) { descriptorsSet->decriptors[std::make_pair(set, binding)] = descriptor; descriptorsSet->num_set = max(descriptorsSet->num_set, set + 1); return *this; }
    private:
        std::shared_ptr<RHIDescriptorsSet> descriptorsSet;
    };
    class RHIDescriptorsSetDestoryer
    {
        friend class RHIDescriptorsSet;
    public:
        RHIDescriptorsSetDestoryer(RHIDescriptorsSet* descriptorsSet) { this->descriptorsSet = descriptorsSet; }
        virtual ~RHIDescriptorsSetDestoryer() { IRHI::Get()->destoryDescriptorsSet(descriptorsSet->location); }
    private:
        RHIDescriptorsSet* descriptorsSet;
    };
}