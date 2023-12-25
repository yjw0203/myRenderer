#pragma once
#include <vulkan/vulkan.h>
#include "rhi/new_rhi/yjw_rhi_define.h"
#include <vector>
#include <optional>

namespace vulkan
{
	template<typename T>
	T& GetGlobalRef();

	typedef VkQueue VkGraphicsQueue;
	typedef VkQueue VkPresentQueue;
	typedef rhi::InitConfig VkInitConfig;

	struct SwapChainInfo
	{
		VkFormat swapchainImageFormat;
		VkExtent2D swapchainExtent;
		std::vector<VkImage> swapchainImages;
		std::vector<VkImageView> swapchainImageViews;
		uint32_t swapchainImageIndex;
	};

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};


#define EXTERN_GLOBAL_REF(T) template T& GetGlobalRef<T>();
#define VK_G(T) GetGlobalRef<T>()

	EXTERN_GLOBAL_REF(VkInitConfig);
	EXTERN_GLOBAL_REF(VkInstance);
	EXTERN_GLOBAL_REF(VkSurfaceKHR);
	EXTERN_GLOBAL_REF(VkPhysicalDevice);
	EXTERN_GLOBAL_REF(QueueFamilyIndices);
	EXTERN_GLOBAL_REF(VkDevice);
	EXTERN_GLOBAL_REF(VkGraphicsQueue);
	EXTERN_GLOBAL_REF(VkPresentQueue);
	EXTERN_GLOBAL_REF(SwapChainInfo);
	EXTERN_GLOBAL_REF(VkSwapchainKHR);

}