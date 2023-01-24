#include "vulkan/vulkan.h"
#include <vector>
#include <optional>

namespace rhi
{
	VkRenderPass createRenderPass();
	VkFramebuffer createFramebuffer(VkRenderPass renderPass, VkImageView* attachments, int attachment_count, int width, int height);
	VkPipelineLayout createPipelineLayout();
	VkPipeline createPipeline(VkPipelineLayout& pipelineLayout, VkRenderPass& renderPass);

}