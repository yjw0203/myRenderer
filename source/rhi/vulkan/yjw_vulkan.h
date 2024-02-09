#pragma once
#include "rhi/new_rhi/yjw_rhi_define.h"
#include "rhi/new_rhi/yjw_rhi.h"

namespace vulkan
{
	class VulkanRHI : public rhi::RHI
	{
		virtual void init(rhi::InitConfig initConfig) override;
		virtual rhi::RHIResourceHandle createResource(const rhi::RHIResourceCreation& creation) override;
		virtual void destoryResource(rhi::RHIResourceHandle resource) override;
		virtual rhi::RHIPSOHandle createPSO(const rhi::PSOCreation& creation) override;
		virtual void destoryPSO(rhi::RHIPSOHandle pso) override;
		virtual rhi::RHIShaderHandle createShader(rhi::RHIShaderCreation& creation) override;
		virtual void destoryShader(rhi::RHIShaderHandle handle) override;
		virtual rhi::RHICommandBufferHandle createCommandBuffer(const rhi::RHICommandBufferCreation& creation) override;
		virtual void destoryCommandBuffer(rhi::RHICommandBufferHandle handle) override;
		virtual rhi::RHIDescriptorSetHandle createDescriptorSet(rhi::RHIPSOHandle pso) override;
		virtual void destoryDescriptorSet(rhi::RHIDescriptorSetHandle descriptorSet) override;
	};
}