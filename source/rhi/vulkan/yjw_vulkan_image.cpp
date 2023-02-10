#include "rhi/yjw_rhi.h"
#include "vulkan/vulkan.h"
#include "yjw_vulkan_context.h"
#include "yjw_vulkan_functions.h"
#include <vector>

namespace rhi
{
    class ImageContext
    {
    public:
        VkImageLayout state = VK_IMAGE_LAYOUT_UNDEFINED;
        VkImage image = nullptr;
        VkDeviceMemory memory = nullptr;

        //Reduce memory fragmentation, use static array.
        //std::vector<VkImageView> views;
        VkImageView views[5];
        int views_count = 0;

        void createImage(Image* item)
        {
            createVkImage(item->width, item->height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, memory);
        }

        ~ImageContext()
        {
            for (int i = 0; i < views_count; i++)
            {
                vkDestroyImageView(g_context.device, views[i], nullptr);
            }
            if (image)vkDestroyImage(g_context.device, image, nullptr);
            if (memory)vkFreeMemory(g_context.device, memory, nullptr);
        }
    };

    Image::Image(int width,int height) : width(width),height(height)
    {
        imageContext = new ImageContext();
        imageContext->createImage(this);
    }
    Image::~Image()
    {
        delete imageContext;
    }


    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(g_context.gpu, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

    }
    void createVkImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        vkCreateImage(g_context.device, &imageInfo, nullptr, &image);

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(g_context.device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        vkAllocateMemory(g_context.device, &allocInfo, nullptr, &imageMemory);

        vkBindImageMemory(g_context.device, image, imageMemory, 0);
    }
}