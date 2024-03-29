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
		renderPassInfo.clearValueCount = 0;
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
		vkTexture->currentState = ResouraceStateAdptor(afterState);
	}
	void VulkanRHI::cmdCopyToSwapchainBackTexture(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIResourceHandle resource)
	{
		VulkanTexture* vulkanTexture = HandleCast<VulkanTexture>(resource);
		VulkanCommandBuffer* commandBuffer = HandleCast<VulkanCommandBuffer>(commandBufferHandle);
		VkCommandBuffer vkCommandBuffer = commandBuffer->commandBuffer;

		VkImage present_image = *vulkanTexture;
		VkImageCopy imageCopyRegion{};
		imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.srcSubresource.layerCount = 1;
		imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.dstSubresource.layerCount = 1;
		imageCopyRegion.extent.width = VK_G(SwapChainInfo).swapchainExtent.width;
		imageCopyRegion.extent.height = VK_G(SwapChainInfo).swapchainExtent.height;
		imageCopyRegion.extent.depth = 1;
		transitionImageLayout(vkCommandBuffer, VK_G(SwapChainInfo).swapchainImages[VK_G(SwapChainInfo).swapchainImageIndex], VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		transitionImageLayout(vkCommandBuffer, present_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
		vkCmdCopyImage(vkCommandBuffer, present_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_G(SwapChainInfo).swapchainImages[VK_G(SwapChainInfo).swapchainImageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);
		transitionImageLayout(vkCommandBuffer, present_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		transitionImageLayout(vkCommandBuffer, VK_G(SwapChainInfo).swapchainImages[VK_G(SwapChainInfo).swapchainImageIndex], VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	}
	void VulkanRHI::cmdClearTexture(rhi::RHICommandBufferHandle commandBufferHandle, rhi::RHIResourceHandle resource)
	{
		if (get_resource_type(resource) == VulkanResourceType::texture)
		{
			VulkanCommandBuffer* vkCommandBuffer = HandleCast<VulkanCommandBuffer>(commandBufferHandle);
			VulkanTexture* vkTexture = HandleCast<VulkanTexture>(resource);
			if (vkTexture->creation.format == VK_FORMAT_D24_UNORM_S8_UINT)
			{
				VkClearDepthStencilValue value{};
				value.depth = 1.0f;
				value.stencil = 0;
				VkImageSubresourceRange range{};
				range.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
				range.baseArrayLayer = 0;
				range.baseMipLevel = 0;
				range.layerCount = 1;
				range.levelCount = 1;
				vkCmdClearDepthStencilImage(vkCommandBuffer->commandBuffer, *vkTexture, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value, 1, &range);
			}
			else
			{
				VkClearColorValue value{ { 0,0,0,0 } };
				VkImageSubresourceRange range{};
				range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				range.baseArrayLayer = 0;
				range.baseMipLevel = 0;
				range.layerCount = 1;
				range.levelCount = 1;
				vkCmdClearColorImage(vkCommandBuffer->commandBuffer, *vkTexture, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value, 1, &range);
			}
		}
	}

}