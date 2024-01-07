#pragma once
#include "vulkan/vulkan.h"
#include "rhi/vulkan/yjw_vulkan_global.h"

namespace vulkan
{
	struct BufferInitConfig
	{
		VkDeviceSize size;
		VkBufferUsageFlags usage;
		VkMemoryPropertyFlags memoryType;
	};

	class Buffer
	{
		friend class BufferPool;
	public:
		operator VkBuffer() { return buffer; }
		operator VkDeviceMemory() { return memory; }
	private:
		Buffer(const BufferInitConfig& inInitConfig) : initConfig(inInitConfig) {}
		const BufferInitConfig initConfig;
		VkBuffer buffer{};
		VkDeviceMemory memory{};
	};
	
	class BufferPool
	{
	public:
		void allocateBuffer(const BufferInitConfig& initConfig, Buffer* buffer);
		void deallocateBuffer(Buffer* buffer);
	};

	EXTERN_GLOBAL_REF(BufferPool);
}