#include "yjw_vulkan_buffer.h"
#include "rhi/vulkan/yjw_vulkan_global.h"

namespace vulkan
{
    uint32_t findMemoryType_(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(VK_G(VkPhysicalDevice), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        return 0;
    }

	void BufferPool::allocateBuffer(const BufferInitConfig& initConfig, Buffer* buffer)
	{
        buffer = new Buffer(initConfig);

        VkBufferCreateInfo desc{};

        desc.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        desc.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        desc.pNext = nullptr;
        desc.usage = initConfig.usage;
        desc.size = initConfig.size;
        desc.flags = 0;
        desc.queueFamilyIndexCount = 0;
        vkCreateBuffer(VK_G(VkDevice), &desc, nullptr, &buffer->buffer);

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(VK_G(VkDevice), buffer->buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType_(memRequirements.memoryTypeBits, initConfig.memoryType);

        vkAllocateMemory(VK_G(VkDevice), &allocInfo, nullptr, &buffer->memory);

        vkBindBufferMemory(VK_G(VkDevice), buffer->buffer, buffer->memory, 0);
	}

	void BufferPool::deallocateBuffer(Buffer* buffer)
	{
        vkDestroyBuffer(VK_G(VkDevice), buffer->buffer, nullptr);
        vkFreeMemory(VK_G(VkDevice), buffer->memory, nullptr);
        delete buffer;
	}
}