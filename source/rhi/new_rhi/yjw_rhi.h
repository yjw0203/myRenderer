#pragma once
#include "yjw_rhi_define.h"
#include "yjw_rhi_resource.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_pso.h"
#include "yjw_rhi_command.h"
#include "yjw_rhi_descriptor_binding.h"

namespace rhi
{
	class RHI
	{
		virtual void init(InitConfig initConfig) = 0;
		virtual RHIResourceHandle createResource(const RHIResourceCreation& creation) = 0;
		virtual void destoryResource(RHIResourceHandle resource) = 0;
		virtual RHIPSOHandle createPSO(const PSOCreation& creation) = 0;
		virtual void destoryPSO(RHIPSOHandle pso) = 0;
		virtual RHIShaderHandle createShader(RHIShaderCreation& creation) = 0;
		virtual void destoryShader(RHIShaderHandle shader) = 0;
		virtual RHICommandBufferHandle createCommandBuffer(const RHICommandBufferCreation& creation) = 0;
		virtual void destoryCommandBuffer(RHICommandBufferHandle commandBuffer) = 0;
		virtual RHIDescriptorSetHandle createDescriptorSet(RHIPSOHandle pso) = 0;
		virtual void destoryDescriptorSet(RHIDescriptorSetHandle descriptorSet) = 0;
	};
}