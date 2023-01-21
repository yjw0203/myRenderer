#include "vulkan/vulkan.h"
#include "yjw_vulkan_delegate.h"
#include <vector>
namespace rhi
{
	class GlobalContext
	{
	public:
		VkInstance instance;
		VkPhysicalDevice gpu;
		VkDevice device;
		VkSurfaceKHR surface;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkSwapchainKHR swapchain;
		VkFormat swapchainImageFormat;
		VkExtent2D swapchainExtent;
		std::vector<VkImage> swapchainImages;
		std::vector<VkImageView> swapchainImageViews;
		uint32_t swapchainImageIndex;
		bool isDebugMode = true;
	};
	extern GlobalContext g_context;

	EXTERN_DELEGATE(OnRHIInitializedDelegate)
}