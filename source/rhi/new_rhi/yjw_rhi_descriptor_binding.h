#pragma once
#include "rhi/common/yjw_rhi_resource_allocator.h"
#include "rhi/new_rhi/yjw_rhi_descriptor.h"
#include "rhi/new_rhi/yjw_rhi.h"
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