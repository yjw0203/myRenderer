#include "yjw_vulkan_resource_ruler.h"

namespace vulkan
{
	bool check_resource_type(const RHIHandle& handle, VulkanResourceType type)
	{
		return handle.TypeId == (int)type;
	}

	void asset_resource_type(const RHIHandle& handle, VulkanResourceType type)
	{
		assert(check_resource_type(handle, type));
	}

	VulkanResourceType get_resource_type(const RHIHandle& handle)
	{
		return (VulkanResourceType)handle.TypeId;
	}
}