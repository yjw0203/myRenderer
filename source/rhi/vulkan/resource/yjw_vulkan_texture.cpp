#include "yjw_vulkan_texture.h"

namespace vulkan
{
    void transitionImageLayout(VkCommandBuffer& commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        if (format == VK_FORMAT_D24_UNORM_S8_UINT)
        {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        }

        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else {
            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            //throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

    }

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
        texture->creation = creation;

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