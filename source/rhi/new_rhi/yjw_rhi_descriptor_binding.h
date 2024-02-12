#pragma once
#include "rhi/common/yjw_rhi_resource_allocator.h"
#include <vector>
namespace rhi
{
    typedef RHIHandle RHIDescriptorSetHandle;

	struct DescriptorWrite
	{
		int setId;
		int slot;
		RHIDescriptorSetHandle descriptor;
	};
}