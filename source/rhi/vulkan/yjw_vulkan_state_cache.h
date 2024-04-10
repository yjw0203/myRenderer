#pragma once
#include "yjw_vulkan_pipeline.h"
#include "yjw_vulkan_resource_binding.h"

namespace rhi
{
    class VulkanStateCache : public RHIObject
    {
    public:
        void SetRenderPipeline(VulkanRenderPipeline* pipeline);
        VulkanRenderPipeline* GetRenderPipeline();
        void SetResourceBinding(VulkanResourceBinding* resourceBinding);
        VulkanResourceBinding* GetResourceBinding();

    private:
        VulkanRenderPipeline* m_current_render_pipeline = nullptr;
        VulkanResourceBinding* m_current_resource_binding = nullptr;
    };
}