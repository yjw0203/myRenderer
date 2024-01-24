#pragma once
#include "vulkan/vulkan.h"
#include "rhi/vulkan/yjw_vulkan_global.h"

namespace vulkan
{
	struct TextureInitConfig
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

	class Texture
	{
		friend class TexturePool;
	public:
		operator VkImage&() { return texture; }
		operator VkDeviceMemory&() { return memory; }
	private:
		Texture(const TextureInitConfig& inInitConfig) : initConfig(inInitConfig) {}
		const TextureInitConfig initConfig;
		VkImage texture{};
		VkDeviceMemory memory{};
	};

	class TexturePool
	{
	public:
		void createTexture(const TextureInitConfig& initConfig, Texture*& texture);
		void destroyTexture(Texture* texture);
	};

	EXTERN_GLOBAL_REF(TexturePool);
}