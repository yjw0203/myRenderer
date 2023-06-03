#include "yjw_rhi_descriptr_set.h"

namespace rhi
{
    RHIAttachmentsSetCreator RHIAttachmentsSet::New(std::shared_ptr<RHIPipeline> pipeline, int width, int height)
    {
        return RHIAttachmentsSetCreator(pipeline, width, height);
    }

    RHIAttachmentsSet::~RHIAttachmentsSet() { RHIAttachmentSetDestoryer destory(this); };

    RHIDescriptorsSetCreator RHIDescriptorsSet::New(std::shared_ptr<RHIPipeline> pipeline)
    {
        return RHIDescriptorsSetCreator(pipeline);
    }

    RHIDescriptorsSet::~RHIDescriptorsSet() { RHIDescriptorsSetDestoryer destory(this); }

    std::shared_ptr<RHIColorAttachment> COLOR_ATTACHMENT(RHIResource* resource, RHIFormat format)
    {
        return std::make_shared<RHIColorAttachment>(resource, format);
    }

    std::shared_ptr<RHIDepthStencilAttachment> DEPTH_STENCIL_ATTACHMENT(RHIResource* resource, RHIFormat format)
    {
        return std::make_shared<RHIDepthStencilAttachment>(resource, format);
    }

    std::shared_ptr<RHIShaderResourceTexture> SHADER_RESOURCE_TEXTURE(RHIResource* resource, RHIFormat format)
    {
        return std::make_shared<RHIShaderResourceTexture>(resource, format);
    }
}