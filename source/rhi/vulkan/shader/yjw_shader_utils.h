#pragma once
#include <vector>
#include "yjw_shader_define.h"
#include "rhi/common/yjw_rhi_resource_allocator.h"

namespace vulkan
{
	struct VulkanShaderAllocateStrategy
	{
		VulkanShader* CreateFunc(const VulkanShaderCreation& creation);
		void DestoryFunc(VulkanShader* shader);
	};

	class ShaderPool
	{
	public:
		VulkanShaderHandle createShader(const VulkanShaderCreation& creation);
		void destoryShader(VulkanShaderHandle handle);
	private:
		ResourceAllocator<VulkanShader, VulkanShaderAllocateStrategy> DefaultAllocator;
	};

	EXTERN_GLOBAL_REF(ShaderPool);
}