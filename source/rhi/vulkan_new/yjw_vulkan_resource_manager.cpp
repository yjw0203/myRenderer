#include "yjw_vulkan_resource_manager.h"
#include "yjw_vulkan_type_conversion.h"
#include "yjw_vulkan_god_objects_manager.h"
#include "yjw_vulkan_command_buffer_manager.h"

namespace rhi
{
    uint32_t findMemoryType_(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(vulkanGod.gpu, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        return 0;
    }

    const VulkanResourceDesc& VulkanResourceLocation::getDesc()
    {
        return desc;
    }

    const VkBuffer* VulkanResourceLocation::getVkBuffer()
    {
        return &buffer;
    }

    const VkImage* VulkanResourceLocation::getVkImage()
    {
        return &image;
    }
    
    const VkDeviceMemory* VulkanResourceLocation::getVkDeviceMemory()
    {
        return &memory;
    }

    RHIResourceLocation* VulkanResourceManager::createResource(const RHIResourceDesc& rhi_desc)
    {
        VulkanResourceLocation* resource = new VulkanResourceLocation();
        VulkanResourceDesc& desc = resource->desc;
        desc.type = rhi_desc.type;
        if (rhi_desc.type == RHIResourceType::buffer)
        {
            desc.bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            desc.bufferInfo.flags = 0;
            desc.bufferInfo.usage = VulkanConverter::convertBufferResourceUsage(rhi_desc.usage);
            desc.bufferInfo.size = rhi_desc.width;
            desc.bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            desc.bufferInfo.queueFamilyIndexCount = 0;
            
            vkCreateBuffer(vulkanGod.device, &desc.bufferInfo, nullptr, &resource->buffer);

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(vulkanGod.device, resource->buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType_(memRequirements.memoryTypeBits, VulkanConverter::convertMemoryProperty(rhi_desc.memoryType));

            vkAllocateMemory(vulkanGod.device, &allocInfo, nullptr, &resource->memory);

            vkBindBufferMemory(vulkanGod.device, resource->buffer, resource->memory, 0);
        }
        else if (rhi_desc.type == RHIResourceType::texture1D)
        {

        }
        else if (rhi_desc.type == RHIResourceType::texture2D)
        {
            desc.imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            desc.imageInfo.imageType = VK_IMAGE_TYPE_2D;
            desc.imageInfo.extent.width = rhi_desc.width;
            desc.imageInfo.extent.height = rhi_desc.height;
            desc.imageInfo.extent.depth = 1;
            desc.imageInfo.mipLevels = 1;
            desc.imageInfo.arrayLayers = 1;
            desc.imageInfo.format = VulkanConverter::convertFormat(rhi_desc.format);
            desc.imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            desc.imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            desc.imageInfo.usage = VulkanConverter::convertImageResourceUsage(rhi_desc.usage);
            desc.imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            desc.imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            vkCreateImage(vulkanGod.device, &desc.imageInfo, nullptr, &resource->image);

            VkMemoryRequirements memRequirements;
            vkGetImageMemoryRequirements(vulkanGod.device, resource->image, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType_(memRequirements.memoryTypeBits, VulkanConverter::convertMemoryProperty(rhi_desc.memoryType));

            vkAllocateMemory(vulkanGod.device, &allocInfo, nullptr, &resource->memory);

            vkBindImageMemory(vulkanGod.device, resource->image, resource->memory, 0);
            
        }
        else if (desc.type == RHIResourceType::texture3D)
        {

        }
        return resource;
    }

    void VulkanResourceManager::deleteResource(RHIResourceLocation*& location)
    {
        VulkanResourceLocation* resource = (VulkanResourceLocation*)location;
        if (resource->desc.type == RHIResourceType::buffer)
        {
            vkDestroyBuffer(vulkanGod.device, resource->buffer, nullptr);
        }
        else if (resource->desc.type == RHIResourceType::texture1D)
        {

        }
        else if (resource->desc.type == RHIResourceType::texture2D)
        {
            vkDestroyImage(vulkanGod.device, resource->image, nullptr);
            vkFreeMemory(vulkanGod.device, resource->memory, nullptr);
        }
        else if (resource->desc.type == RHIResourceType::texture3D)
        {

        }
        delete location;
        location = nullptr;
    }

    void VulkanResourceCopyer::copyResource(VkCommandBuffer commandBuffer, RHIResource* src, RHIResource* dst)
    {
        if (src->getDesc().type == RHIResourceType::buffer && dst->getDesc().type == RHIResourceType::texture2D)
        {
            copyBufferToTexture2D(commandBuffer, src, dst);
        }
    }

    void VulkanResourceCopyer::copyBufferToTexture2D(VkCommandBuffer commandBuffer, RHIResource* src, RHIResource* dst)
    {
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            (unsigned int)dst->getDesc().width,
            (unsigned int)dst->getDesc().height,
            1
        };

        vkCmdCopyBufferToImage(commandBuffer, *((VulkanResourceLocation*)src->resourceLocation)->getVkBuffer(), *((VulkanResourceLocation*)dst->resourceLocation)->getVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    }

    void VulkanResourceWriter::writeResourceImmidiately(RHIResource* resource, void* data, int size)
    {
        if (resource->getDesc().type == RHIResourceType::buffer)
        {
            writeBufferImmidiately(resource, data, size);
        }
    }

    void VulkanResourceWriter::writeBufferImmidiately(RHIResource* resource, void* data, int size)
    {
        void* buffer_map;
        vkMapMemory(vulkanGod.device, *((VulkanResourceLocation*)resource->resourceLocation)->getVkDeviceMemory(), 0, size, 0, &buffer_map);
        memcpy(buffer_map, data, static_cast<size_t>(size));
        vkUnmapMemory(vulkanGod.device, *((VulkanResourceLocation*)resource->resourceLocation)->getVkDeviceMemory());
    }
}