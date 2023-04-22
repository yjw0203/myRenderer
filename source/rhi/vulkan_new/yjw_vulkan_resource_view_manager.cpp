#include "yjw_vulkan_resource_view_manager.h"
#include "yjw_vulkan_resource_manager.h"
#include "yjw_vulkan_type_conversion.h"
#include "yjw_vulkan_god_objects_manager.h"

namespace rhi
{
    const VkImageView* VulkanResourceViewLocation::getVkImageView()
    {
        return &view;
    }

    RHIResourceViewLocation* VulkanResourceViewManager::createResourceView(ResourceViewType type, RHIResource* resource, RHIFormat view_format)
    {
        VulkanResourceLocation* vulkan_resource = (VulkanResourceLocation*)resource->resourceLocation;
        VulkanResourceViewLocation* vulkan_resource_view = new VulkanResourceViewLocation();
        vulkan_resource_view->type = resource->getDesc().type;
        if (resource->getDesc().type == RHIResourceType::buffer)
        {

        }
        else if(resource->getDesc().type == RHIResourceType::texture1D)
        {

        }
        else if (resource->getDesc().type == RHIResourceType::texture2D)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = *vulkan_resource->getVkImage();
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = VulkanConverter::convertFormat(view_format);
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            switch (type)
            {
            case rtv:createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; break;
            case dsv:createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT; break;
            case srv:createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; break;
            case uav:createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; break;
            default:createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; break;
            }

            vkCreateImageView(vulkanGod.device, &createInfo, nullptr, &vulkan_resource_view->view);
        }
        else if (resource->getDesc().type == RHIResourceType::texture3D)
        {

        }

        return vulkan_resource_view;
    }

    void VulkanResourceViewManager::deleteResourceView(RHIResourceViewLocation*& location)
    {
        VulkanResourceViewLocation* resource = (VulkanResourceViewLocation*)location;
        if (resource->type == RHIResourceType::buffer)
        {

        }
        else if (resource->type == RHIResourceType::texture1D)
        {

        }
        else if (resource->type == RHIResourceType::texture2D)
        {
            vkDestroyImageView(vulkanGod.device, resource->view, nullptr);
        }
        else if (resource->type == RHIResourceType::texture3D)
        {

        }

        delete location;
        location = nullptr;
    }

}