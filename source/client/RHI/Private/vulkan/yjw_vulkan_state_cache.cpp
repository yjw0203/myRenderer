#include "Private/vulkan/yjw_vulkan_state_cache.h"

namespace rhi
{
    void VulkanStateCache::SetRenderPipeline(VulkanRenderPipeline* pipeline)
    {
        m_current_render_pipeline = pipeline;
    }

    VulkanRenderPipeline* VulkanStateCache::GetRenderPipeline()
    {
        return m_current_render_pipeline;
    }

    void VulkanStateCache::SetResourceBinding(VulkanResourceBinding* resourceBinding)
    {
        m_current_resource_binding = resourceBinding;
    }

    VulkanResourceBinding* VulkanStateCache::GetResourceBinding()
    {
        return m_current_resource_binding;
    }
}