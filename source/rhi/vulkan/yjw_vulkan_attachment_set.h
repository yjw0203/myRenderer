#pragma once
#include "rhi/vulkan/yjw_vulkan_resource_ruler.h"
#include "rhi/vulkan/yjw_vulkan_global.h"
#include "rhi/vulkan/yjw_vulkan_pso.h"
#include "rhi/vulkan/resource/yjw_vulkan_descriptor.h"

namespace vulkan
{
	class VulkanAttachmentSetCreation
	{
	public:
		VulkanPSOHandle psoHandle;
		std::vector<VulkanDescriptorHandle> descriptors;
	};

	class VulkanAttachmentSet
	{
	public:
		static const VulkanResourceType TypeId = VulkanResourceType::attachmentSet;
		typedef VulkanAttachmentSetCreation Creation;
		operator VkFramebuffer& () { return frameBuffer; }
		VkFramebuffer frameBuffer;
	};

	typedef ResourceHandle<VulkanAttachmentSet> VulkanAttachmentSetHandle;

	struct DefaultVulkanVulkanAttachmentSetAllocateStrategy
	{
		VulkanAttachmentSet* CreateFunc(VulkanAttachmentSetCreation& creation);
		void DestoryFunc(VulkanAttachmentSet* descriptorSet);
	};

	class VulkanAttachmentSetPool
	{
	public:
		VulkanAttachmentSetHandle createAttachmentSet(VulkanAttachmentSetCreation creation);
		void destoryAttachmentSet(VulkanAttachmentSetHandle handle);
	private:
		ResourceAllocator<VulkanAttachmentSet, DefaultVulkanVulkanAttachmentSetAllocateStrategy> DefaultAllocator;
	};

	EXTERN_GLOBAL_REF(VulkanAttachmentSetPool);
}