#pragma once
#include "rhi/new_rhi/yjw_rhi_define.h"
#include "rhi/new_rhi/yjw_rhi.h"

namespace vulkan
{
	class VulkanRHI : public rhi::RHI
	{
		virtual void init(rhi::InitConfig initConfig) override;
		virtual rhi::RHIResourceHandle createResource(const rhi::RHIResourceCreation& creation) override;
		virtual void destoryResource(rhi::RHIResourceHandle resource) override;
		virtual rhi::RHIResourceState getResourceState(rhi::RHIResourceHandle resource) override;
		virtual rhi::RHIPSOHandle createPSO(rhi::PSOCreation& creation) override;
		virtual void destoryPSO(rhi::RHIPSOHandle pso) override;
		virtual rhi::RHIShaderHandle createShader(rhi::RHIShaderCreation& creation) override;
		virtual void destoryShader(rhi::RHIShaderHandle handle) override;
		virtual rhi::RHICommandBufferHandle createCommandBuffer(const rhi::RHICommandBufferCreation& creation) override;
		virtual void destoryCommandBuffer(rhi::RHICommandBufferHandle handle) override;
		virtual rhi::RHIDescriptorSetHandle createDescriptorSet(rhi::RHIPSOHandle pso) override;
		virtual void destoryDescriptorSet(rhi::RHIDescriptorSetHandle descriptorSet) override;
		virtual rhi::RHIDescriptorHandle createDescriptor(rhi::RHIDescriptorCreation creation) override;
		virtual void destoryDescriptor(rhi::RHIDescriptorHandle descriptor) override;
		virtual rhi::RHIAttachmentSetHandle createAttachmentSet(rhi::RHIAttachmentSetCreation creation) override;
		virtual void destoryAttachmentSet(rhi::RHIAttachmentSetHandle attachmentSet) override;

		//update
		virtual void writeDescriptorSet(rhi::RHIDescriptorSetHandle descriptorSet, rhi::DescriptorWrite* writes, int count) override;
		virtual void updateResource(rhi::RHIResourceHandle resourceHandle, void* data, int offset, int size) override;
		virtual void writeResourceBarrierImmediately(rhi::RHIResourceHandle resourceHandle, rhi::RHIResourceState beforeState, rhi::RHIResourceState afterState) override;

		//cmd
		virtual void cmdBeginPass(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIPSOHandle psoHandle, rhi::RHIAttachmentSetHandle attachmentSetHandle) override;
		virtual void cmdEndPass(rhi::RHICommandBufferHandle commandBufferHandle) override;
		virtual void cmdBindDescriptorSet(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIDescriptorSetHandle descriptorSetHandle) override;
		virtual void cmdBindVertexBuffer(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIResourceHandle bufferHandle) override;
		virtual void cmdBindIndexBuffer(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIResourceHandle bufferHandle) override;
		virtual void cmdDrawIndex(rhi::RHICommandBufferHandle commandBufferHandle, int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance) override;
		virtual void cmdDraw(rhi::RHICommandBufferHandle commandBufferHandle, int vertexCount, int instanceCount, int firstVertex, int firstInstance) override;
		virtual void cmdResourceBarrier(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIResourceHandle resourceHandle, rhi::RHIResourceState beforeState, rhi::RHIResourceState afterState) override;
		virtual void cmdCopyToSwapchainBackTexture(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIResourceHandle resource) override;
		virtual void cmdClearTexture(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIResourceHandle resource) override;
	
		virtual void submitCommandBuffer(rhi::RHICommandBufferHandle commandBufferHandle) override;
		virtual void resetCommandBuffer(rhi::RHICommandBufferHandle commandBufferHandle) override;

		virtual void waitForIdle() override;
		virtual void present() override;
	};
}