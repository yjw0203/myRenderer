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
    }

    const VulkanResourceDesc& VulkanResourceLocation::getDesc()
    {
        return desc;
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
            desc.imageInfo.usage = VulkanConverter::convertResourceUsage(rhi_desc.usage);
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
            

            {
                //will be remove after resource barrier done
                if (rhi_desc.usage == 0)
                {
                    VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().beginOneTimeCommandBuffer();

                    transitionImageLayout(commandBuffer, resource->image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

                    VulkanCommandBufferAllocater::Get().endOneTimeCommandBuffer(commandBuffer);
                }
            }
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
}