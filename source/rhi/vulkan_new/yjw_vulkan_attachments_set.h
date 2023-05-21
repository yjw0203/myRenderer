#pragma once
#include "rhi/rhi/yjw_rhi_descriptr_set.h"
#include "vulkan/vulkan.h"

namespace rhi
{
    struct VulkanAttachmentsSetLocation : RHIAttachmentsSetLocation
    {
        VkFramebuffer frameBuffer;
    };

    class VulkanAttachmentsSet
    {
    public:
        static std::shared_ptr<RHIAttachmentsSetLocation> createAttachmentsSet(std::shared_ptr<RHIAttachmentsSet> attachmentsSet);
        static void destoryAttachmentsSet(std::shared_ptr<RHIAttachmentsSetLocation> location);
    };
}