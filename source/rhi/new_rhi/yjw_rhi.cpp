#include "yjw_rhi.h"
#include "rhi/vulkan/yjw_vulkan.h"

namespace rhi
{
	void init(InitConfig initConfig)
	{
		vulkan::init(initConfig);
	}
}