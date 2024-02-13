#pragma once
#include "rhi/vulkan/yjw_vulkan_resource_ruler.h"
#include "rhi/vulkan/yjw_vulkan_global.h"
#include "rhi/vulkan/yjw_vulkan_pso.h"

namespace vulkan
{
	class VulkanDescariptorSetCreation
	{
	public:
		VulkanDescariptorSetCreation() {}
		VulkanPSOHandle pso;
	};

	class VulkanDescriptorSet
	{
	public:
		static const VulkanResourceType TypeId = VulkanResourceType::descriptorSet;
		typedef VulkanDescariptorSetCreation Creation;
		VulkanPSOHandle psoHandle;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
		VkDescriptorSet getDescriptorSet(int index) { return descriptorSets[index]; }
		VkDescriptorPool getDescriptorPool() { return descriptorPool; }
	};

	typedef ResourceHandle<VulkanDescriptorSet> VulkanDescriptorSetHandle;

	struct DefaultVulkanDescriptorSetAllocateStrategy
	{
		VulkanDescriptorSet* CreateFunc(VulkanDescariptorSetCreation& creation);
		void DestoryFunc(VulkanDescriptorSet* descriptorSet);
	};

	class VulkanDescriptorSetPool
	{
	public:
		VulkanDescriptorSetHandle createDescriptorSet(VulkanDescariptorSetCreation& creation);
		void destoryDescriptorSet(VulkanDescriptorSetHandle handle);
	private:
		ResourceAllocator<VulkanDescriptorSet, DefaultVulkanDescriptorSetAllocateStrategy> DefaultAllocator;
	};

	EXTERN_GLOBAL_REF(VulkanDescriptorSetPool);
}