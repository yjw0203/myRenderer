#include "yjw_vulkan_texture.h"

namespace vulkan
{
    uint32_t findTextureMemoryType_(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(VK_G(VkPhysicalDevice), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        return 0;
    }

    VulkanTexture* DefaultVulkanTextureAllocateStrategy::CreateFunc(const VulkanTextureCreation& creation)
    {
        VulkanTexture* texture = new VulkanTexture();

        VkImageCreateInfo desc{};
        desc.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        desc.imageType = VK_IMAGE_TYPE_2D;
        desc.extent.width = creation.width;
        desc.extent.height = creation.height;
        desc.extent.depth = creation.depth;
        desc.mipLevels = creation.mipLevels;
        desc.arrayLayers = creation.arrayLayers;
        desc.format = creation.format;
        desc.tiling = VK_IMAGE_TILING_OPTIMAL;
        desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        desc.usage = creation.usage;
        desc.samples = VK_SAMPLE_COUNT_1_BIT;
        desc.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        desc.queueFamilyIndexCount = 0;

        vkCreateImage(VK_G(VkDevice), &desc, nullptr, &texture->texture);

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(VK_G(VkDevice), texture->texture, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findTextureMemoryType_(memRequirements.memoryTypeBits, creation.memoryType);

        vkAllocateMemory(VK_G(VkDevice), &allocInfo, nullptr, &texture->memory);

        vkBindImageMemory(VK_G(VkDevice), texture->texture, texture->memory, 0);
        return texture;
    }
    
    void DefaultVulkanTextureAllocateStrategy::DestoryFunc(VulkanTexture* resource)
    {
        vkDestroyImage(VK_G(VkDevice), resource->texture, nullptr);
        vkFreeMemory(VK_G(VkDevice), resource->memory, nullptr);
        delete resource;
    }

    VulkanTextureHandle TexturePool::createTexture(const VulkanTextureCreation& creation)
	{
        return DefaultAllocator.create(creation);
	}

	void TexturePool::destroyTexture(VulkanTextureHandle handle)
	{
        DefaultAllocator.destory(handle);
	}

}