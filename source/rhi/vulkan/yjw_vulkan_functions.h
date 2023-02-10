#include "vulkan/vulkan.h"
#include <vector>
#include <optional>

namespace rhi
{
    VkRenderPass createRenderPass();
    VkFramebuffer createFramebuffer(VkRenderPass renderPass, VkImageView* attachments, int attachment_count, int width, int height);
    VkPipelineLayout createPipelineLayout();
    VkPipeline createPipeline(VkPipelineLayout& pipelineLayout, VkRenderPass& renderPass, RasterizationState rasterizationState);
    void createVkImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

}