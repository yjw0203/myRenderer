#include "yjw_rpi.h"

namespace rpi
{
	RPIResource createGpuVertexBuffer(int size)
	{
		rhi::RHIResourceCreation creation{};
		creation.type = rhi::ResourceType::buffer;
		creation.width = size;
		creation.usage = (int)rhi::ResourceUsageBits::allow_vertex_buffer | (int)rhi::ResourceUsageBits::allow_transfer_dst | (int)rhi::ResourceUsageBits::deny_shader_resource;
		creation.memoryType = rhi::MemoryType::default_;
		return rhi::GpuDevice->createResource(creation);
	}
	RPIResource createGpuIndexBuffer(int size)
	{
		rhi::RHIResourceCreation creation{};
		creation.type = rhi::ResourceType::buffer;
		creation.width = size;
		creation.usage = (int)rhi::ResourceUsageBits::allow_index_buffer | (int)rhi::ResourceUsageBits::allow_transfer_dst | (int)rhi::ResourceUsageBits::deny_shader_resource;
		creation.memoryType = rhi::MemoryType::default_;
		return rhi::GpuDevice->createResource(creation);
	}
	RPIResource createGpuIndirectBuffer(int size)
	{
		rhi::RHIResourceCreation creation{};
		creation.type = rhi::ResourceType::buffer;
		creation.width = size;
		creation.usage = (int)rhi::ResourceUsageBits::allow_indirect_buffer | (int)rhi::ResourceUsageBits::allow_transfer_dst | (int)rhi::ResourceUsageBits::deny_shader_resource;
		creation.memoryType = rhi::MemoryType::default_;
		return rhi::GpuDevice->createResource(creation);
	}
	RPIResource createUploadBuffer(int size)
	{
		rhi::RHIResourceCreation creation{};
		creation.type = rhi::ResourceType::buffer;
		creation.width = size;
		creation.usage = (int)rhi::ResourceUsageBits::allow_indirect_buffer | (int)rhi::ResourceUsageBits::allow_transfer_src;
		creation.memoryType = rhi::MemoryType::upload;
		return rhi::GpuDevice->createResource(creation);
	}
	RPIResource createDefaultTexture2D(int width, int height, RPIFormat format, int mipLevels /* = 1*/)
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
	RPIResource createDepthStencilTexture2D(int width, int height, RPIFormat format)
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
	RPIResource createUploadTexture2D(int width, int height, RPIFormat format)
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
	void destoryResource(RPIResource resource)
	{
		rhi::GpuDevice->destoryResource(resource);
	}
	RPIShader createShader(const char* name)
	{
		rhi::RHIShaderCreation creation{};
		creation.name = std::string(name);
		return rhi::GpuDevice->createShader(creation);
	}
	void destoryShader(RPIShader shader)
	{
		rhi::GpuDevice->destoryShader(shader);
	}
	RPIDescriptor createDescriptor(RPIResource resource, RPIDescriptorType descriptorType, RPIFormat format)
	{
		rhi::RHIDescriptorCreation creation{};
		creation.type = descriptorType;
		creation.resource = resource;
		creation.format = format;
		return rhi::GpuDevice->createDescriptor(creation);
	}
	void destoryDescriptor(RPIDescriptor descriptor)
	{
		rhi::GpuDevice->destoryDescriptor(descriptor);
	}
	RPIDescriptorSet createDescriptorSet(RPIPipeline pipeline)
	{
		return rhi::GpuDevice->createDescriptorSet(pipeline);
	}
	void destoryDescriptorSet(RPIDescriptorSet descriptorSet)
	{
		rhi::GpuDevice->destoryDescriptorSet(descriptorSet);
	}
}