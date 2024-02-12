#include "yjw_rpi_pipeline.h"

namespace rpi
{
	void RPIPipelineCreator::addVertexAttribute(RPIFormat format)
	{
		creation.vertex_binding.vertex_formats.push_back(format);
	}
	void RPIPipelineCreator::addColorAttachment(RPIFormat format)
	{
		creation.attachment_binding.attachment_formats.push_back(format);
	}
	void RPIPipelineCreator::addDepthStencilAttachment(RPIFormat format)
	{
		creation.attachment_binding.depth_stencil_format = format;
	}
	void RPIPipelineCreator::addDescriptor(RPIShaderType shaderType, int setId, int slotId, RPIDescriptorType descriptorType)
	{
		rhi::DescriptorBinding binding{};
		binding.shaderStage = shaderType;
		binding.setId = setId;
		binding.slotId = slotId;
		binding.descriptorType = descriptorType;
		creation.descriptor_layout_binding.descriptor_bindings.push_back(binding);
	}
	void RPIPipelineCreator::addShaderEntry(RPIShaderType shaderType, RPIShader shader, const char* entryName)
	{
		rhi::ShaderEntry entry{};
		entry.shaderType = shaderType;
		entry.shader = shader;
		entry.entryName = std::string(entryName);
		creation.shader_binding.shader_entries.push_back(entry);
	}
	RPIPipeline RPIPipelineCreator::create()
	{
		//default raster state
		creation.rasterization_state_binding.polygonMode = rhi::PolygonMode::fill;
		creation.rasterization_state_binding.cullMode = rhi::CullMode::back;
		creation.rasterization_state_binding.frontFace = rhi::FrontFace::counter_clockwise;
		return rhi::GpuDevice->createPSO(creation);
	}
	void RPIPipelineCreator::clear()
	{
		creation = rhi::PSOCreation{};
	}
	void RPIPipelineDestoryer::destory()
	{
		rhi::GpuDevice->destoryPSO(pipeline);
	}
}