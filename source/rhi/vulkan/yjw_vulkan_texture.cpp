#include "yjw_vulkan_texture.h"
#include "vulkan/vulkan.h"
#include "yjw_vulkan_type_conversation.h"
#include "yjw_vulkan_resource_cast.h"

namespace rhi
{
    uint32_t findTextureMemoryType_(VkPhysicalDevice gpu, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(gpu, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        return 0;
    }

    VulkanTexture::VulkanTexture(VulkanDevice* device, const RHITextureDescriptor& desc)
        : VulkanDeviceObject(device)
        , RHITexture(desc)
    {
        m_current_layout = VK_IMAGE_LAYOUT_UNDEFINED;

        VkImageCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        switch (desc.resourceType)
        {
        case RHIResourceType::texture1D:
            createInfo.imageType = VK_IMAGE_TYPE_1D;
            createInfo.extent.width = desc.width;
            createInfo.extent.height = 1;
            createInfo.extent.depth = 1;
            createInfo.arrayLayers = 1;
            break;
        case RHIResourceType::texture2D:
            createInfo.imageType = VK_IMAGE_TYPE_2D;
            createInfo.extent.width = desc.width;
            createInfo.extent.height = desc.height;
            createInfo.extent.depth = 1;
            createInfo.arrayLayers = 1;
            break;
        case RHIResourceType::texture3D:
            createInfo.imageType = VK_IMAGE_TYPE_3D;
            createInfo.extent.width = desc.width;
            createInfo.extent.height = desc.height;
            createInfo.extent.depth = desc.depthOrArraySize;
            createInfo.arrayLayers = 1;
            break;
        case RHIResourceType::textureCube:
            createInfo.imageType = VK_IMAGE_TYPE_2D;
            createInfo.extent.width = desc.width;
            createInfo.extent.height = desc.height;
            createInfo.extent.depth = 1;
            createInfo.arrayLayers = 6;
            break;
        case RHIResourceType::texture2DArray:
            createInfo.imageType = VK_IMAGE_TYPE_2D;
            createInfo.extent.width = desc.width;
            createInfo.extent.height = desc.height;
            createInfo.extent.depth = 1;
            createInfo.arrayLayers = desc.depthOrArraySize;
            break;
        }
        createInfo.mipLevels = desc.miplevels;
        createInfo.format = ConvertFormatToVkFormat(desc.format);
        createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        createInfo.usage = ConvertImageUsageToVkImageUsage(desc.usage);
        createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.initialLayout = m_current_layout;

        vkCreateImage(device->GetNativeDevice(), &createInfo, nullptr, &m_image);

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device->GetNativeDevice(), m_image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findTextureMemoryType_(device->GetGpu(), memRequirements.memoryTypeBits, ConvertMemoryTypeToVkMemoryPropertyFlags(desc.memoryType));

        vkAllocateMemory(device->GetNativeDevice(), &allocInfo, nullptr, &m_memory);

        vkBindImageMemory(device->GetNativeDevice(), m_image, m_memory, 0);
        m_b_create_from_exist_image = false;
    }

    VulkanTexture::VulkanTexture(VulkanDevice* device, const RHITextureDescriptor& desc, VkImage existImage, VkImageLayout currentLayout)
        : VulkanDeviceObject(device)
        , RHITexture(desc)
        , m_current_layout(currentLayout)
    {
        m_image = existImage;
        m_b_create_from_exist_image = true;
    }

    VulkanTexture::~VulkanTexture()
    {
        if (m_b_create_from_exist_image)
        {
        }
        else
        {
            vkFreeMemory(GetDevice()->GetNativeDevice(), m_memory, nullptr);
            vkDestroyImage(GetDevice()->GetNativeDevice(), m_image, nullptr);
        }
    }

    VkImage VulkanTexture::GetVkImage()
    {
        return m_image;
    }

    VkDeviceMemory VulkanTexture::GetVkDeviceMemory()
    {
        return m_memory;
    }

    int VulkanTexture::GetWidth()
    {
        return GetDesc().width;
    }

    int VulkanTexture::GetHeight()
    {
        return GetDesc().height;
    }

    void VulkanTexture::TransitionState(VkCommandBuffer commandBuffer, VkImageLayout newLayout)
    {
        if (newLayout == m_current_layout)
        {
            return;
        }
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = m_current_layout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = m_image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        if (ConvertFormatToVkFormat(GetDesc().format) == VK_FORMAT_D24_UNORM_S8_UINT)
        {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        }

        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (m_current_layout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (m_current_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
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

        m_current_layout = newLayout;
    }

    VulkanTextureView::VulkanTextureView(VulkanDevice* device, const RHITextureViewDescriptor& desc)
        : VulkanDeviceObject(device)
        , RHITextureView(desc)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = GetTexture()->GetVkImage();
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = ConvertFormatToVkFormat(desc.format);
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        if (createInfo.format == VK_FORMAT_D24_UNORM_S8_UINT)
        {
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        }
        else
        {
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        }

        vkCreateImageView(device->GetNativeDevice(), &createInfo, nullptr, &m_view);

        desc.texture->retain(this);
    }

    VulkanTextureView::~VulkanTextureView()
    {
        vkDestroyImageView(GetDevice()->GetNativeDevice(), m_view, nullptr);
        GetDesc().texture->release();
    }

    VulkanTexture* VulkanTextureView::GetTexture()
    {
        return ResourceCast(GetDesc().texture);
    }

    VkImageView VulkanTextureView::GetVkImageView()
    {
        return m_view;
    }

    VkFormat VulkanTextureView::GetVkFormat()
    {
        return ConvertFormatToVkFormat(GetDesc().format);
    }

    int VulkanTextureView::GetWidth()
    {
        return GetTexture()->GetWidth();
    }

    int VulkanTextureView::GetHeight()
    {
        return GetTexture()->GetHeight();
    }
}