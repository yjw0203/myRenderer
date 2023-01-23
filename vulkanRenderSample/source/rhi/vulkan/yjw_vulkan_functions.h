#include "vulkan/vulkan.h"
#include <vector>
#include <optional>

namespace rhi
{
	VkRenderPass createRenderPass();
	VkPipelineLayout createPipelineLayout();
	VkPipeline createPipeline(VkPipelineLayout& pipelineLayout, VkRenderPass& renderPass);

}