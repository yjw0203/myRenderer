#include "../yjw_rhi.h"
#include "yjw_vulkan_context.h"

namespace rhi
{
	void initializeRasterizationState()
	{
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

		g_pipelineStateTable.rasterizationState[RasterizationState::Rasterization_default] = rasterizer;
	}

	REGISTER_DELEGATE(OnRHIInitializedDelegate, initializeRasterizationState)
}