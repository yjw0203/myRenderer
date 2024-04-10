#include "yjw_rpi_pipeline.h"

namespace rpi
{
	/*
	rhi::DepthStencilStateBinding getDepthStencilState(RPIDepthStencilState state)
	{
		rhi::DepthStencilStateBinding binding{};
		binding.depthCompareOption = rhi::RHICompareOption::less_or_equal;
		if (state == RPIDepthStencilState::no_depth_no_stencil)
		{
			binding.depthTest = false;
			binding.depthWrite = false;
		}
		else if(state == RPIDepthStencilState::depth_read)
		{
			binding.depthTest = true;
			binding.depthWrite = false;
		}
		else if (state == RPIDepthStencilState::depth_write)
		{
			binding.depthTest = false;
			binding.depthWrite = true;
		}
		else if (state == RPIDepthStencilState::depth_read_and_wirte)
		{
			binding.depthTest = true;
			binding.depthWrite = true;
		}
		return binding;
	}

	rhi::PSORasterizationStateBinding getRasterizationState(RPIRasterizationState state)
	{
		rhi::PSORasterizationStateBinding binding{};
		binding.polygonMode = rhi::PolygonMode::fill;
		binding.frontFace = rhi::FrontFace::clockwise;
		if (state == RPIRasterizationState::default_)
		{
			binding.cullMode = rhi::CullMode::back;
		}
		else if (state == RPIRasterizationState::only_back_face)
		{
			binding.cullMode = rhi::CullMode::front;
		}
		else if (state == RPIRasterizationState::both_face)
		{
			binding.cullMode = rhi::CullMode::none;
		}
		return binding;
	}

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
	void RPIPipelineCreator::addDepthStencilState(RPIDepthStencilState state)
	{
		depthStencilState = state;
	}
	void RPIPipelineCreator::addRasterizationState(RPIRasterizationState state)
	{
		rasterizationState = state;
	}
	void RPIPipelineCreator::flushCacheStateToCreation()
	{
		creation.depth_stencil_state_binding = getDepthStencilState(depthStencilState);
		creation.rasterization_state_binding = getRasterizationState(rasterizationState);
	}
	RPIPipeline RPIPipelineCreator::create()
	{
		flushCacheStateToCreation();
		return rhi::GpuDevice->createPSO(creation);
	}
	void RPIPipelineCreator::clear()
	{
		creation = rhi::PSOCreation{};
	}
	void RPIPipelineDestoryer::destory()
	{
		rhi::GpuDevice->destoryPSO(pipeline);
	}*/
}