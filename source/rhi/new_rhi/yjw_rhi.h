#pragma once
#include "yjw_rhi_define.h"
#include "yjw_rhi_resource.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_pso.h"
#include "yjw_rhi_command.h"
#include "yjw_rhi_descriptor_binding.h"
#include "yjw_rhi_descriptor.h"
#include "yjw_rhi_attachment_set.h"

namespace rhi
{
	class RHI
	{
	public:
		virtual void init(InitConfig initConfig) = 0;
		virtual RHIResourceHandle createResource(const RHIResourceCreation& creation) = 0;
		virtual void destoryResource(RHIResourceHandle resource) = 0;
		virtual RHIResourceState getResourceState(RHIResourceHandle resource) = 0;
		virtual RHIPSOHandle createPSO(PSOCreation& creation) = 0;
		virtual void destoryPSO(RHIPSOHandle pso) = 0;
		virtual RHIShaderHandle createShader(RHIShaderCreation& creation) = 0;
		virtual void destoryShader(RHIShaderHandle shader) = 0;
		virtual RHICommandBufferHandle createCommandBuffer(const RHICommandBufferCreation& creation) = 0;
		virtual void destoryCommandBuffer(RHICommandBufferHandle commandBuffer) = 0;
		virtual RHIDescriptorSetHandle createDescriptorSet(RHIPSOHandle pso) = 0;
		virtual void destoryDescriptorSet(RHIDescriptorSetHandle descriptorSet) = 0;
		virtual RHIDescriptorHandle createDescriptor(RHIDescriptorCreation creation) = 0;
		virtual void destoryDescriptor(RHIDescriptorHandle descriptor) = 0;
		virtual RHIAttachmentSetHandle createAttachmentSet(RHIAttachmentSetCreation creation) = 0;
		virtual void destoryAttachmentSet(RHIAttachmentSetHandle attachmentSet) = 0;

		//update
		virtual void writeDescriptorSet(RHIDescriptorSetHandle descriptorSet, DescriptorWrite* write, int count) = 0;
		virtual void updateResource(RHIResourceHandle resourceHandle, void* data, int offset, int size) = 0;
		virtual void writeResourceBarrierImmediately(RHIResourceHandle resourceHandle, RHIResourceState beforeState, RHIResourceState afterState) = 0;

		//cmd
		virtual void cmdBeginPass(RHICommandBufferHandle commandBufferHandle,RHIPSOHandle psoHandle, RHIAttachmentSetHandle attachmentSetHandle) = 0;
		virtual void cmdEndPass(RHICommandBufferHandle commandBufferHandle) = 0;
		virtual void cmdBindDescriptorSet(RHICommandBufferHandle commandBufferHandle, RHIDescriptorSetHandle descriptorSetHandle) = 0;
		virtual void cmdBindVertexBuffer(RHICommandBufferHandle commandBufferHandle, RHIResourceHandle bufferHandle) = 0;
		virtual void cmdBindIndexBuffer(RHICommandBufferHandle commandBufferHandle, RHIResourceHandle bufferHandle) = 0;
		virtual void cmdDrawIndex(RHICommandBufferHandle commandBufferHandle, int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance) = 0;
		virtual void cmdDraw(RHICommandBufferHandle commandBufferHandle, int vertexCount, int instanceCount, int firstVertex, int firstInstance) = 0;
		virtual void cmdResourceBarrier(RHICommandBufferHandle commandBufferHandle, RHIResourceHandle resourceHandle, RHIResourceState beforeState, RHIResourceState afterState) = 0;
		virtual void cmdCopyToSwapchainBackTexture(RHICommandBufferHandle commandBufferHandle, RHIResourceHandle resource) = 0;
		virtual void cmdClearTexture(RHICommandBufferHandle commandBufferHandle, RHIResourceHandle resource) = 0;
	
		virtual void submitCommandBuffer(RHICommandBufferHandle commandBufferHandle) = 0;
		virtual void resetCommandBuffer(RHICommandBufferHandle commandBufferHandle) = 0;

		virtual void waitForIdle() = 0;
		virtual void present() = 0;
	};
	extern RHI* GpuDevice;
}