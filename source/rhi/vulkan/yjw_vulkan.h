#pragma once
#include "rhi/new_rhi/yjw_rhi_define.h"
#include "rhi/new_rhi/yjw_rhi.h"

namespace vulkan
{
	class VulkanRHI : public rhi::RHI
	{
		virtual void init(rhi::InitConfig initConfig) override;
		virtual void createResource(const rhi::ResourceInitConfig& initConfig, rhi::Resource* resource) override;
		virtual void destoryResource(rhi::Resource* resource) override;
	};
}