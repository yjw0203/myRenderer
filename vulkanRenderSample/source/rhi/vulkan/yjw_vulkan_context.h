#pragma once

#include "../yjw_rhi.h"
#include "vulkan/vulkan.h"
#include "yjw_vulkan_delegate.h"
#include <vector>
#include <optional>
namespace rhi
{
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	class GlobalContext
	{
	public:
		VkInstance instance;
		VkPhysicalDevice gpu;
		VkDevice device;
		VkSurfaceKHR surface;

		//queue
		QueueFamilyIndices queueFamilyIndices;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		//command
		std::vector<VkCommandBuffer> commandBufferList;

		//swapchain
		VkSwapchainKHR swapchain;
		VkFormat swapchainImageFormat;
		VkExtent2D swapchainExtent;
		std::vector<VkImage> swapchainImages;
		std::vector<VkImageView> swapchainImageViews;
		uint32_t swapchainImageIndex;

		//debug
		bool isDebugMode = false;
	};
	extern GlobalContext g_context;

	class PipelineStateTable
	{
	public:
		VkPipelineRasterizationStateCreateInfo rasterizationState[RasterizationState::Rasterization_count];

	};
	extern PipelineStateTable g_pipelineStateTable;

	EXTERN_DELEGATE(OnRHIInitializedDelegate)
}