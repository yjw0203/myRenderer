#pragma once
#include "yjw_rhi_define.h"
#include "yjw_rhi_resource.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_pso.h"
#include "yjw_rhi_command.h"

namespace rhi
{
	class RHI
	{
		virtual void init(InitConfig initConfig) = 0;
		virtual void createResource(const ResourceInitConfig& initConfig, Resource*& resource) = 0;
		virtual void destoryResource(Resource*& resource) = 0;
		virtual void createShader(const ShaderInitConfig& initConfig, Shader*& shader) = 0;
		virtual void destoryShader(Shader*& shader) = 0;
		virtual void createPSO(const PSOInitConfig& initConfig, PSO*& pso) = 0;
		virtual void destoryPSO(PSO*& pso) = 0;
		virtual void createCommandBuffer(const CommandBufferInitConfig& initConfig, CommandBuffer*& commandBuffer) = 0;
		virtual void destoryCommandBuffer(CommandBuffer*& commandBuffer) = 0;
	};
}