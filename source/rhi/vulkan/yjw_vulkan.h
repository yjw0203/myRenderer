#pragma once
#include "rhi/new_rhi/yjw_rhi_define.h"
#include "rhi/new_rhi/yjw_rhi.h"

namespace vulkan
{
	class VulkanRHI : public rhi::RHI
	{
		virtual void init(rhi::InitConfig initConfig) override;
		virtual void createResource(const rhi::ResourceInitConfig& initConfig, rhi::Resource*& resource) override;
		virtual void destoryResource(rhi::Resource*& resource) override;
		virtual void createShader(const rhi::ShaderInitConfig& initConfig, rhi::Shader*& shader) override;
		virtual void destoryShader(rhi::Shader*& shader) override;
		virtual void createPSO(const rhi::PSOInitConfig& initConfig, rhi::PSO*& pso) override;
		virtual void destoryPSO(rhi::PSO*& pso) override;
		virtual void createCommandBuffer(const rhi::CommandBufferInitConfig& initConfig, rhi::CommandBuffer*& commandBuffer) override;
		virtual void destoryCommandBuffer(rhi::CommandBuffer*& commandBuffer) override;
	};
}