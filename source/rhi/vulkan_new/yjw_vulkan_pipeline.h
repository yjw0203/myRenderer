#pragma once
#include "rhi/rhi/yjw_rhi_pipeline.h"
#include "vulkan/vulkan.h"

namespace rhi
{
    struct VulkanPipelineLocation : RHIPipelineLocation
    {
        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        VkPipeline pipeline;
    };

    class VulkanPipeline
    {
    public:
        static std::shared_ptr<RHIPipelineLocation> createPipeline(std::shared_ptr<RHIPipeline> pipeline);
        static void destoryPipeline(std::shared_ptr<RHIPipelineLocation> location);
    };
}