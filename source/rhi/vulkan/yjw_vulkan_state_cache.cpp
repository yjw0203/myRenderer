#include "yjw_vulkan_state_cache.h"

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
}