#pragma once
#include "rhi/common/yjw_rhi_resource_allocator.h"
#include "cassert"
namespace vulkan
{
	enum class VulkanResourceType : char
	{
		buffer = 0,
		texture,
		descriptor,
		pso,
		shader,
		commandBuffer,
		descriptorSet,
		attachmentSet,
	};

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

	template<typename Type>
	Type* HandleCast(RHIHandle handle)
	{
		return((ResourceHandle<Type>)handle).Get();
	}
}