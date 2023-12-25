#pragma once
#include <vector>

namespace vulkan
{
	struct DefaultPipelineRasterizationState : VkPipelineRasterizationStateCreateInfo
	{
		DefaultPipelineRasterizationState()
		{
			sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			depthClampEnable = VK_FALSE;
			rasterizerDiscardEnable = VK_FALSE;
			polygonMode = VK_POLYGON_MODE_FILL;
			lineWidth = 1.0f;
			cullMode = VK_CULL_MODE_FRONT_BIT;
			frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			depthBiasEnable = VK_FALSE;
			flags = 0;
			pNext = nullptr;
		}
	};

	struct DefaultPipelineDepthStencilState : VkPipelineDepthStencilStateCreateInfo
	{
		DefaultPipelineDepthStencilState()
		{
			sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthTestEnable = VK_FALSE;
			depthWriteEnable = VK_FALSE;
			depthCompareOp = VK_COMPARE_OP_LESS;
			depthBoundsTestEnable = VK_FALSE;
			stencilTestEnable = VK_FALSE;
			flags = 0;
			pNext = nullptr;
		}
	};

	struct DefaultPipelineInputAssemblyState : VkPipelineInputAssemblyStateCreateInfo
	{
		DefaultPipelineInputAssemblyState()
		{
			sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			primitiveRestartEnable = VK_FALSE;
			flags = 0;
			pNext = nullptr;
		}
	};

	struct DefaultPipelineTessellationState : VkPipelineTessellationStateCreateInfo
	{
		DefaultPipelineTessellationState()
		{
			sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
			patchControlPoints = 0;
			flags = 0;
			pNext = nullptr;
		}
	};

	struct DefaultPipelineViewportState : VkPipelineViewportStateCreateInfo
	{
		DefaultPipelineViewportState()
		{
			sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportCount = 1;
			pViewports = nullptr;
			scissorCount = 1;
			pScissors = nullptr;
			flags = 0;
			pNext = nullptr;
		}
	};

	struct DefaultPipelineMultisampleState : VkPipelineMultisampleStateCreateInfo
	{
		DefaultPipelineMultisampleState()
		{
			sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			sampleShadingEnable = VK_FALSE;
			rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			pSampleMask = nullptr;
			alphaToCoverageEnable = VK_FALSE;
			alphaToOneEnable = VK_FALSE;
			flags = 0;
			pNext = nullptr;
		}
	};

	struct DefaultPipelineDynamicState : VkPipelineDynamicStateCreateInfo
	{
		DefaultPipelineDynamicState()
		{
			static std::vector<VkDynamicState> dynamicStates = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};
			sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
			pDynamicStates = dynamicStates.data();
		}
	};
}