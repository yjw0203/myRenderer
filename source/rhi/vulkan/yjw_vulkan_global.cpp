#include "yjw_vulkan_global.h"
#include <vulkan/vulkan.h>

namespace vulkan
{
	template<typename T>
	T& GetGlobalRef()
	{
		static T instance;
		return instance;
	}
}