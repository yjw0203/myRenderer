#pragma once
#include "RHI/rhi/yjw_rhi_define.h"
#include "yjw_vulkan_state_cache.h"
#include "yjw_vulkan_render_pass.h"
#include "yjw_vulkan_device.h"

namespace rhi
{
    class VulkanContext : public RHIContext, VulkanDeviceObject
    {
    public:
        VulkanContext(VulkanDevice* device);
    private:
        VulkanRenderPass m_render_pass;
        VulkanStateCache m_state_cache;
    };
}