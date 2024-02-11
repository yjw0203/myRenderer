#include "yjw_vulkan.h"
#include "vulkan/vulkan.h"
#include "yjw_vulkan_descriptor_set.h"
#include "rhi/vulkan/resource/yjw_vulkan_descriptor.h"
#include "rhi/vulkan/command/yjw_vulkan_command_buffer.h"
#include "rhi/vulkan/yjw_vulkan_attachment_set.h"
#include "yjw_vulkan_resource_ruler.h"
#include "rhi/vulkan/adaptor/yjw_property_adaptor.h"

namespace vulkan
{
	//cmd
	void VulkanRHI::cmdBeginPass(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIPSOHandle psoHandle, rhi::RHIAttachmentSetHandle attachmentSetHandle)
	{
		VkCommandBuffer commandBuffer = HandleCast<VulkanCommandBuffer>(commandBufferHandle)->commandBuffer;
		VulkanPSO* pso = HandleCast<VulkanPSO>(psoHandle);
		VulkanAttachmentSet* attachmentSet = HandleCast<VulkanAttachmentSet>(attachmentSetHandle);

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = pso->renderPass;
		renderPassInfo.framebuffer = attachmentSet->frameBuffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VK_G(SwapChainInfo).swapchainExtent;

		renderPassInfo.pClearValues = nullptr;

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pso->pso);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = VK_G(SwapChainInfo).swapchainExtent.width;
		viewport.height = VK_G(SwapChainInfo).swapchainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VK_G(SwapChainInfo).swapchainExtent;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}
	void VulkanRHI::cmdEndPass(rhi::RHICommandBufferHandle commandBufferHandle)
	{
		VkCommandBuffer commandBuffer = HandleCast<VulkanCommandBuffer>(commandBufferHandle)->commandBuffer;
		vkCmdEndRenderPass(commandBuffer);
	}
	void VulkanRHI::cmdBindDescriptorSet(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIDescriptorSetHandle descriptorSetHandle)
	{
		VkCommandBuffer commandBuffer = HandleCast<VulkanCommandBuffer>(commandBufferHandle)->commandBuffer;
		VulkanDescriptorSet* descriptorSet = HandleCast<VulkanDescriptorSet>(descriptorSetHandle);
		VulkanPSO* pso = HandleCast<VulkanPSO>(descriptorSet->psoHandle);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pso->pipelineLayout, 0, descriptorSet->descriptorSets.size(), descriptorSet->descriptorSets.data(), 0, nullptr);
	}
	void VulkanRHI::cmdBindVertexBuffer(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIResourceHandle bufferHandle)
	{
		VkCommandBuffer commandBuffer = HandleCast<VulkanCommandBuffer>(commandBufferHandle)->commandBuffer;
		VulkanBuffer* vkBuffer = HandleCast<VulkanBuffer>(bufferHandle);
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vkBuffer->buffer, &offset);
	}
	void VulkanRHI::cmdBindIndexBuffer(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIResourceHandle bufferHandle)
	{
		VkCommandBuffer commandBuffer = HandleCast<VulkanCommandBuffer>(commandBufferHandle)->commandBuffer;
		VulkanBuffer* vkBuffer = HandleCast<VulkanBuffer>(bufferHandle);
		vkCmdBindIndexBuffer(commandBuffer, vkBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);
	}
	void VulkanRHI::cmdDrawIndex(rhi::RHICommandBufferHandle commandBufferHandle, int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance)
	{
		VkCommandBuffer commandBuffer = HandleCast<VulkanCommandBuffer>(commandBufferHandle)->commandBuffer;
		vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}
	void VulkanRHI::cmdDraw(rhi::RHICommandBufferHandle commandBufferHandle, int vertexCount, int instanceCount, int firstVertex, int firstInstance)
	{
		VkCommandBuffer commandBuffer = HandleCast<VulkanCommandBuffer>(commandBufferHandle)->commandBuffer;
		vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}
	void VulkanRHI::cmdResourceBarrier(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIResourceHandle resourceHandle, rhi::RHIResourceState beforeState, rhi::RHIResourceState afterState)
	{
		VkCommandBuffer commandBuffer = HandleCast<VulkanCommandBuffer>(commandBufferHandle)->commandBuffer;
		VulkanTexture* vkTexture = HandleCast<VulkanTexture>(resourceHandle);
		transitionImageLayout(commandBuffer, *vkTexture, vkTexture->creation.format, ResouraceStateAdptor(beforeState), ResouraceStateAdptor(afterState));
	}
}