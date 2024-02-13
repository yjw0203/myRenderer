#pragma once
#include "yjw_rhi_pso.h"
#include "yjw_rhi_descriptor.h"
#include "rhi/common/yjw_rhi_resource_allocator.h"
#include <vector>
namespace rhi
{ 
	struct RHIAttachmentBinding
	{
		int slot;
		bool isDepth;
		RHIDescriptorHandle descriptor;
	};

	struct RHIAttachmentSetCreation
	{
		RHIPSOHandle psoHandle;
		std::vector<RHIAttachmentBinding> attachments;
	};

	typedef RHIHandle RHIAttachmentSetHandle;
}