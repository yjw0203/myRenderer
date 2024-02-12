#include "yjw_rpi.h"

namespace rpi
{
	void RPIInit(int width, int height, void* window)
	{
		rhi::InitConfig initConfig{};
		initConfig.debug_mode = true;
		initConfig.width = width;
		initConfig.height = height;
		initConfig.window = window;
		rhi::GpuDevice->init(initConfig);
	}

	RPIBuffer RPICreateGpuVertexBuffer(int size)
	{
		rhi::RHIResourceCreation creation{};
		creation.type = rhi::ResourceType::buffer;
		creation.width = size;
		creation.usage = (int)rhi::ResourceUsageBits::allow_vertex_buffer | (int)rhi::ResourceUsageBits::allow_transfer_dst | (int)rhi::ResourceUsageBits::deny_shader_resource;
		creation.memoryType = rhi::MemoryType::default_;
		return rhi::GpuDevice->createResource(creation);
	}
	RPIBuffer RPICreateGpuIndexBuffer(int size)
	{
		rhi::RHIResourceCreation creation{};
		creation.type = rhi::ResourceType::buffer;
		creation.width = size;
		creation.usage = (int)rhi::ResourceUsageBits::allow_index_buffer | (int)rhi::ResourceUsageBits::allow_transfer_dst | (int)rhi::ResourceUsageBits::deny_shader_resource;
		creation.memoryType = rhi::MemoryType::default_;
		return rhi::GpuDevice->createResource(creation);
	}
	RPIBuffer RPICreateGpuIndirectBuffer(int size)
	{
		rhi::RHIResourceCreation creation{};
		creation.type = rhi::ResourceType::buffer;
		creation.width = size;
		creation.usage = (int)rhi::ResourceUsageBits::allow_indirect_buffer | (int)rhi::ResourceUsageBits::allow_transfer_dst | (int)rhi::ResourceUsageBits::deny_shader_resource;
		creation.memoryType = rhi::MemoryType::default_;
		return rhi::GpuDevice->createResource(creation);
	}
	RPIBuffer RPICreateUploadBuffer(int size)
	{
		rhi::RHIResourceCreation creation{};
		creation.type = rhi::ResourceType::buffer;
		creation.width = size;
		creation.usage = (int)rhi::ResourceUsageBits::allow_indirect_buffer | (int)rhi::ResourceUsageBits::allow_transfer_src;
		creation.memoryType = rhi::MemoryType::upload;
		return rhi::GpuDevice->createResource(creation);
	}
	RPITexture RPICreateDefaultTexture2D(int width, int height, RPIFormat format, int mipLevels /* = 1*/)
	{
		rhi::RHIResourceCreation creation{};
		creation.type = rhi::ResourceType::texture2D;
		creation.width = width;
		creation.height = height;
		creation.depthOrArraySize = 1;
		creation.miplevels = mipLevels;
		creation.format = format;
		creation.usage = (int)rhi::ResourceUsageBits::allow_render_target | (int)rhi::ResourceUsageBits::allow_transfer_src | (int)rhi::ResourceUsageBits::allow_transfer_dst | (int)rhi::ResourceUsageBits::allow_unordered_access;
		creation.memoryType = rhi::MemoryType::default_;
		return rhi::GpuDevice->createResource(creation);
	}
	RPITexture RPICreateDepthStencilTexture2D(int width, int height, RPIFormat format)
	{
		rhi::RHIResourceCreation creation{};
		creation.type = rhi::ResourceType::texture2D;
		creation.width = width;
		creation.height = height;
		creation.depthOrArraySize = 1;
		creation.miplevels = 1;
		creation.format = format;
		creation.usage = (int)rhi::ResourceUsageBits::allow_depth_stencil | (int)rhi::ResourceUsageBits::allow_transfer_src | (int)rhi::ResourceUsageBits::allow_transfer_dst;
		creation.memoryType = rhi::MemoryType::default_;
		return rhi::GpuDevice->createResource(creation);
	}
	RPITexture RPICreateUploadTexture2D(int width, int height, RPIFormat format)
	{
		rhi::RHIResourceCreation creation{};
		creation.type = rhi::ResourceType::texture2D;
		creation.width = width;
		creation.height = height;
		creation.depthOrArraySize = 1;
		creation.miplevels = 1;
		creation.format = format;
		creation.usage = (int)rhi::ResourceUsageBits::allow_transfer_src;
		creation.memoryType = rhi::MemoryType::default_;
		return rhi::GpuDevice->createResource(creation);
	}
	void RPIDestoryResource(RPIResource resource)
	{
		rhi::GpuDevice->destoryResource(resource);
	}
	RPIShader RPICreateShader(const char* name)
	{
		rhi::RHIShaderCreation creation{};
		creation.name = std::string(name);
		return rhi::GpuDevice->createShader(creation);
	}
	void RPIDestoryShader(RPIShader shader)
	{
		rhi::GpuDevice->destoryShader(shader);
	}
	RPIDescriptor RPICreateDescriptor(RPIResource resource, RPIDescriptorType descriptorType, RPIFormat format)
	{
		rhi::RHIDescriptorCreation creation{};
		creation.type = descriptorType;
		creation.resource = resource;
		creation.format = format;
		return rhi::GpuDevice->createDescriptor(creation);
	}
	void RPIDestoryDescriptor(RPIDescriptor descriptor)
	{
		rhi::GpuDevice->destoryDescriptor(descriptor);
	}
	RPIDescriptorSet RPICreateDescriptorSet(RPIPipeline pipeline)
	{
		return rhi::GpuDevice->createDescriptorSet(pipeline);
	}
	void RPIDestoryDescriptorSet(RPIDescriptorSet descriptorSet)
	{
		rhi::GpuDevice->destoryDescriptorSet(descriptorSet);
	}
	RPICommandBuffer RPICreateCommandBuffer()
	{
		rhi::RHICommandBufferCreation creation{};
		return rhi::GpuDevice->createCommandBuffer(creation);
	}
	void RPIDestoryCommandBuffer(RPICommandBuffer commandBuffer)
	{
		rhi::GpuDevice->destoryCommandBuffer(commandBuffer);
	}
	void RPISubmitCommandBuffer(RPICommandBuffer commandBuffer)
	{
		rhi::GpuDevice->submitCommandBuffer(commandBuffer);
	}
	void RPIResetCommandBuffer(RPICommandBuffer commandBuffer)
	{
		rhi::GpuDevice->resetCommandBuffer(commandBuffer);
	}
	void RPICmdCopyToSwapchainBackTexture(RPICommandBuffer commandBuffer, RPITexture texture)
	{
		rhi::GpuDevice->cmdCopyToSwapchainBackTexture(commandBuffer, texture);
	}
	void RPIPresent()
	{
		rhi::GpuDevice->present();
	}
	void RPIUpdateResource(RPIResource resource, void* data,int offset, int size)
	{
		rhi::GpuDevice->updateResource(resource, data, offset, size);
	}
}