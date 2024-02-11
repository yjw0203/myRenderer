#pragma once
#include "rhi/new_rhi/yjw_rhi_resource.h"
#include "rhi/vulkan/resource/yjw_vulkan_resource_header.h"
#include "rhi/new_rhi/yjw_rhi_descriptor.h"
#include "rhi/vulkan/resource/yjw_vulkan_descriptor.h"
#include "rhi/new_rhi/yjw_rhi_attachment_set.h"
#include "rhi/vulkan/yjw_vulkan_attachment_set.h"
namespace vulkan
{
    class VulkanDescriptorCreationAdaptor
    {
    public:
        VulkanDescriptorCreationAdaptor(const rhi::RHIDescriptorCreation& creation) : payload(creation) {}
        operator VulkanDescriptorCreation();
    private:
        const rhi::RHIDescriptorCreation& payload;
    };

    class VulkanAttachmentSetCreationAdaptor
    {
    public:
        VulkanAttachmentSetCreationAdaptor(const rhi::RHIAttachmentSetCreation& creation) : payload(creation) {}
        operator VulkanAttachmentSetCreation();
    private:
        const rhi::RHIAttachmentSetCreation& payload;
    };
}