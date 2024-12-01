#pragma once
#include "Engine/RHI/Private/vulkan/yjw_vulkan_pipeline.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_resource_binding.h"

namespace rhi
{
    class VulkanStateCache : public RHIObject
    {
    public:
        void SetRenderPipeline(VulkanRenderPipeline* pipeline);
        VulkanRenderPipeline* GetRenderPipeline();
        void SetComputePipeline(VulkanComputePipeline* pipeline);
        VulkanComputePipeline* GetComputePipeline();
        void SetResourceBinding(VulkanResourceBinding* resourceBinding);
        VulkanResourceBinding* GetResourceBinding();
        void SetPrimitiveBinding(VulkanPrimitiveBinding* primitiveBinding);
        VulkanPrimitiveBinding* GetPrimitiveBinding();
    private:
        VulkanRenderPipeline* m_current_render_pipeline = nullptr;
        VulkanComputePipeline* m_current_compute_pipeline = nullptr;
        VulkanResourceBinding* m_current_resource_binding = nullptr;
        VulkanPrimitiveBinding* m_current_primitive_binding = nullptr;
    };
}