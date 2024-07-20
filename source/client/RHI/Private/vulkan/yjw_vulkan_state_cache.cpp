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

    void VulkanStateCache::SetComputePipeline(VulkanComputePipeline* pipeline)
    {
        m_current_compute_pipeline = pipeline;
    }

    VulkanComputePipeline* VulkanStateCache::GetComputePipeline()
    {
        return m_current_compute_pipeline;
    }

    void VulkanStateCache::SetResourceBinding(VulkanResourceBinding* resourceBinding)
    {
        m_current_resource_binding = resourceBinding;
    }

    VulkanResourceBinding* VulkanStateCache::GetResourceBinding()
    {
        return m_current_resource_binding;
    }

    void VulkanStateCache::SetPrimitiveBinding(VulkanPrimitiveBinding* primitiveBinding)
    {
        m_current_primitive_binding = primitiveBinding;
    }

    VulkanPrimitiveBinding* VulkanStateCache::GetPrimitiveBinding()
    {
        return m_current_primitive_binding;
    }
}