#include "yjw_rhi.h"
#include "rhi/vulkan/yjw_vulkan.h"

namespace rhi
{
	RHI* GpuDevice = new vulkan::VulkanRHI();
}