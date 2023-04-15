#pragma once
#include "rhi/rhi/yjw_rhi_draw_template.h"
#include "vulkan/vulkan.h"
#include "yjw_vulkan_pipeline_state.h"
namespace rhi
{
    class VulkanDefaultDrawTemplateBuildCache : public DefaultDrawTemplateBuildCache
    {
    public:
        VkCommandBuffer commandBuffer;
        VulkanPipelineStateObject pso;
    };

    class VulkanDefaultDrawTemplateBuilder
    {
    public:
        static void excuteDrawTemplate(const DefaultDrawTemplate* draw_template);
        static DefaultDrawTemplateBuildCache* buildDrawTemplateCache(DefaultDrawTemplate* draw_template);
        static void deleteDrawTemplateCache(DefaultDrawTemplateBuildCache*& draw_template_cache);
    };
}