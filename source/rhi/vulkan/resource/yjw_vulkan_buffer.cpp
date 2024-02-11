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

    VulkanBuffer* DefaultVulkanBufferAllocateStrategy::CreateFunc(const VulkanBufferCreation& creation)
    {
        VulkanBuffer* buffer = new VulkanBuffer();
        buffer->size = creation.size;

        VkBufferCreateInfo desc{};
        desc.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        desc.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        desc.pNext = nullptr;
        desc.usage = creation.usage;
        desc.size = creation.size;
        desc.flags = 0;
        desc.queueFamilyIndexCount = 0;
        vkCreateBuffer(VK_G(VkDevice), &desc, nullptr, &buffer->buffer);

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(VK_G(VkDevice), buffer->buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType_(memRequirements.memoryTypeBits, creation.memoryType);

        vkAllocateMemory(VK_G(VkDevice), &allocInfo, nullptr, &buffer->memory);

        vkBindBufferMemory(VK_G(VkDevice), buffer->buffer, buffer->memory, 0);
        return buffer;
    }

    void DefaultVulkanBufferAllocateStrategy::DestoryFunc(VulkanBuffer* resource)
    {
        vkDestroyBuffer(VK_G(VkDevice), resource->buffer, nullptr);
        vkFreeMemory(VK_G(VkDevice), resource->memory, nullptr);
        delete resource;
    }

    VulkanBufferHandle BufferPool::allocateBuffer(const VulkanBufferCreation& creation)
	{
        return DefaultAllocator.create(creation);
	}

	void BufferPool::deallocateBuffer(VulkanBufferHandle handle)
	{
        DefaultAllocator.destory(handle);
	}
}