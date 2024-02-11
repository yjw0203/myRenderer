#pragma once
#include "vulkan/vulkan.h"
#include "rhi/vulkan/yjw_vulkan_global.h"
#include "rhi/common/yjw_rhi_resource_allocator.h"
#include "rhi/vulkan/yjw_vulkan_resource_ruler.h"

namespace vulkan
{

	void transitionImageLayout(VkCommandBuffer& commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	struct VulkanTextureCreation
	{
		VkImageType type;
		uint32_t width;
		uint32_t height;
		uint32_t depth;
		uint32_t mipLevels;
		uint32_t arrayLayers;
		VkFormat format;
		VkImageUsageFlags usage;
		VkMemoryPropertyFlags memoryType;
	};

	class VulkanTexture
	{
	public:
		static const VulkanResourceType TypeId = VulkanResourceType::texture;
		typedef VulkanTextureCreation Creation;
		operator VkImage&() { return texture; }
		operator VkDeviceMemory&() { return memory; }
		VkImage texture{};
		VkDeviceMemory memory{};
		VulkanTextureCreation creation{};
	};
	typedef ResourceHandle<VulkanTexture> VulkanTextureHandle;

	struct DefaultVulkanTextureAllocateStrategy
	{
		VulkanTexture* CreateFunc(const VulkanTextureCreation& creation);
		void DestoryFunc(VulkanTexture* resource);
	};

	class TexturePool
	{
	public:
		VulkanTextureHandle createTexture(const VulkanTextureCreation& initConfig);
		void destroyTexture(VulkanTextureHandle handle);

	private:
		ResourceAllocator<VulkanTexture, DefaultVulkanTextureAllocateStrategy> DefaultAllocator;
	};

	EXTERN_GLOBAL_REF(TexturePool);
}