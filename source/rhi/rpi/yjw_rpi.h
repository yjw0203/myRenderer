#pragma once
#include "yjw_rpi_define.h"

namespace rpi
{
	RPIResource createGpuVertexBuffer(int size);
	RPIResource createGpuIndexBuffer(int size);
	RPIResource createGpuIndirectBuffer(int size);
	RPIResource createUploadBuffer(int size);
	RPIResource createDefaultTexture2D(int width, int height, RPIFormat format,int mipLevels = 1);
	RPIResource createDepthStencilTexture2D(int width, int height, RPIFormat format);
	RPIResource createUploadTexture2D(int width, int height, RPIFormat format);
	void destoryResource(RPIResource resource);

	RPIShader createShader(const char* name);
	void destoryShader(RPIShader shader);

	RPIDescriptor createDescriptor(RPIResource resource, RPIDescriptorType descriptorType, RPIFormat format);
	void destoryDescriptor(RPIDescriptor descriptor);

	RPIDescriptorSet createDescriptorSet(RPIPipeline pipeline);
	void destoryDescriptorSet(RPIDescriptorSet descriptorSet);
}