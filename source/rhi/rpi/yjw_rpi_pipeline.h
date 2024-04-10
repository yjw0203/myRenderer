#pragma once
#include "yjw_rpi_define.h"

namespace rpi
{
	/*
	class RPIPipelineCreator
	{
	public:
		void addVertexAttribute(RPIFormat format);
		void addColorAttachment(RPIFormat format);
		void addDepthStencilAttachment(RPIFormat format);
		void addDescriptor(RPIShaderType shaderType, int setId, int slotId, RPIDescriptorType descriptorType);
		void addShaderEntry(RPIShaderType shaderType, RPIShader shader, const char* entryName);
		void addDepthStencilState(RPIDepthStencilState state);
		void addRasterizationState(RPIRasterizationState state);
		RPIPipeline create();
		void clear();
	private:
		void flushCacheStateToCreation();
		RPIDepthStencilState depthStencilState = RPIDepthStencilState::no_depth_no_stencil;
		RPIRasterizationState rasterizationState = RPIRasterizationState::default_;
		rhi::PSOCreation creation{};
	};

	class RPIPipelineDestoryer
	{
	public:
		RPIPipelineDestoryer(RPIPipeline pipeline) :pipeline(pipeline) {}
		void destory();
	private:
		RPIPipeline pipeline;
	};*/
}