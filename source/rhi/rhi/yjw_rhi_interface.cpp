#include "yjw_rhi_interface.h"
#include "rhi/vulkan_new/yjw_vulkan_rhi.h"
namespace rhi
{
    IRHI* IRHI::Get()
    {
        static IVulkanRHI instance;
        return &instance;
    }

}