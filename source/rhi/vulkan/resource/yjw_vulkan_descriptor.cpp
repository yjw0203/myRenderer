#include "yjw_vulkan_descriptor.h"
#include "rhi/vulkan/yjw_vulkan_global.h"
#include "rhi/vulkan/adaptor/yjw_property_adaptor.h"

namespace vulkan
{
	VulkanDescriptor* DefaultVulkanDescriptorAllocateStrategy::CreateFunc(VulkanDescriptorCreation& creation)
	{
        VulkanDescriptor* descriptor = new VulkanDescriptor();
        descriptor->resource_type = creation.resource_type;
        descriptor->descriptor_type = creation.descriptor_type;
        if (creation.resource_type == VulkanResourceType::buffer)
        {
            VulkanBuffer* buffer = creation.handle.buffer.Get();
            descriptor->memory = *buffer;
            descriptor->payload.bufferView.buffer = *buffer;
            descriptor->payload.bufferView.offset = 0;
            descriptor->payload.bufferView.size = buffer->size;
        }
        else if(creation.resource_type == VulkanResourceType::texture)
        {
            VulkanTexture* texture = creation.handle.texture.Get();
            descriptor->memory = *texture;
            descriptor->payload.imageView.width = texture->creation.width;
            descriptor->payload.imageView.height = texture->creation.height;

            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = *texture;
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = creation.format;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            if (creation.format == VK_FORMAT_D24_UNORM_S8_UINT)
            {
                createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
                if (creation.descriptor_type == VulkanDescriptorType::shader_resource_texture)
                {
                    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;//hack
                }
            }
            else
            {
                createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            }

            vkCreateImageView(VK_G(VkDevice), &createInfo, nullptr, &descriptor->payload.imageView.view);
        }
        return descriptor;
	}

	void DefaultVulkanDescriptorAllocateStrategy::DestoryFunc(VulkanDescriptor* resource)
	{
        if (resource->resource_type == VulkanResourceType::buffer)
        {
        }
        else if (resource->resource_type == VulkanResourceType::texture)
        {
            vkDestroyImageView(VK_G(VkDevice), resource->payload.imageView.view, nullptr);
        }
        delete resource;
	}

	VulkanDescriptorHandle VulkanDescriptorPool::create(VulkanDescriptorCreation& creation)
	{
        return DefaultAllocator.create(creation);
	}

	void VulkanDescriptorPool::destory(VulkanDescriptorHandle descriptor)
	{
        DefaultAllocator.destory(descriptor);
	}
}