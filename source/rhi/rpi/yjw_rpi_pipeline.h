#pragma once
#include "yjw_rpi_define.h"

namespace rpi
{
	class RPIPipelineCreator
	{
	public:
		void addVertexAttribute(RPIFormat format);
		void addColorAttachment(RPIFormat format);
		void addDepthStencilAttachment(RPIFormat format);
		void addDescriptor(RPIShaderType shaderType, int setId, int slotId, RPIDescriptorType descriptorType);
		void addShaderEntry(RPIShader shader, const char* entryName);
		RPIPipeline create();
		void clear();
	private:
		rhi::PSOCreation creation{};
	};

	class RPIPipelineDestoryer
	{
	public:
		RPIPipelineDestoryer(RPIPipeline pipeline) :pipeline(pipeline) {}
		void destory();
	private:
		RPIPipeline pipeline;
	};
}