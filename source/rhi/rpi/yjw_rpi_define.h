#pragma once
#include "rhi/new_rhi/yjw_rhi_header.h"

namespace rpi
{
	typedef rhi::RHIResourceHandle RPIResource;
	typedef RPIResource RPIBuffer;
	typedef RPIResource RPITexture;
	typedef rhi::RHIDescriptorHandle RPIDescriptor;
	typedef rhi::RHIAttachmentSetHandle RPIAttachmentSet;
	typedef rhi::RHIDescriptorSetHandle RPIDescriptorSet;
	typedef rhi::RHIPSOHandle RPIPipeline;
	typedef rhi::RHICommandBufferHandle RPICommandBuffer;
	typedef rhi::RHIShaderHandle RPIShader;

	typedef rhi::Format RPIFormat;
	typedef rhi::DescriptorType RPIDescriptorType;
	typedef rhi::ShaderType RPIShaderType;
	typedef rhi::RHIResourceState RPIResourceState;

	enum RPIDepthStencilState
	{
		no_depth_no_stencil,
		depth_read_and_wirte,
		depth_read,
		depth_write
	};
}