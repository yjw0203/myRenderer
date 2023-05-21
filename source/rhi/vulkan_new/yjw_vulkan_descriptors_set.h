#pragma once
#include "rhi/rhi/yjw_rhi_descriptr_set.h"
#include "vulkan/vulkan.h"

namespace rhi
{
    struct VulkanDescriptorsSetLocation : RHIDescriptorsSetLocation
    {
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
    };

    class VulkanDescritporsSet
    {
    public:
        static std::shared_ptr<RHIDescriptorsSetLocation> createDescriptorsSet(std::shared_ptr<RHIDescriptorsSet> descriptorsSet);
        static void destoryDescriptorsSet(std::shared_ptr<RHIDescriptorsSetLocation> location);
    };
}