#pragma once
#include "vulkan/vulkan.h"

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
		VkBuffer getNative() { return buffer; }
	private:
		VkBuffer buffer;
		VkDeviceMemory memory;
	};
	
	class BufferPool
	{
	public:
		void allocateBuffer(const BufferInitConfig* initConfig, Buffer* buffer);
		void deallocateBuffer(Buffer* buffer);
	};
}