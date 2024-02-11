#pragma once
#include "vulkan/vulkan.h"
#include "rhi/vulkan/yjw_vulkan_global.h"
#include "rhi/common/yjw_rhi_resource_allocator.h"
#include "rhi/vulkan/yjw_vulkan_resource_ruler.h"
namespace vulkan
{

	struct VulkanBufferCreation
	{
		VkDeviceSize size;
		VkBufferUsageFlags usage;
		VkMemoryPropertyFlags memoryType;
	};

	class VulkanBuffer
	{
	public:
		static const VulkanResourceType TypeId = VulkanResourceType::buffer;
		typedef VulkanBufferCreation Creation;
		operator VkBuffer() { return buffer; }
		operator VkDeviceMemory() { return memory; }
		VulkanBuffer() {}
		VkDeviceSize size{};
		VkBuffer buffer{};
		VkDeviceMemory memory{};
	};
	typedef ResourceHandle<VulkanBuffer> VulkanBufferHandle;

	struct DefaultVulkanBufferAllocateStrategy
	{
		VulkanBuffer* CreateFunc(const VulkanBufferCreation& creation);
		void DestoryFunc(VulkanBuffer* resource);
	};
	
	class BufferPool
	{
	public:
		VulkanBufferHandle allocateBuffer(const VulkanBufferCreation& creation);
		void deallocateBuffer(VulkanBufferHandle buffer);
	private:
		ResourceAllocator<VulkanBuffer, DefaultVulkanBufferAllocateStrategy> DefaultAllocator;
	};

	EXTERN_GLOBAL_REF(BufferPool);
}