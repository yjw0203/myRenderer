#pragma once
#include "yjw_vulkan_pipeline.h"

namespace rhi
{
    class VulkanStateCache : public RHIObject
    {
    public:
        void SetRenderPipeline(VulkanRenderPipeline* pipeline);
        VulkanRenderPipeline* GetRenderPipeline();

    private:
        VulkanRenderPipeline* m_current_render_pipeline = nullptr;
    };
}