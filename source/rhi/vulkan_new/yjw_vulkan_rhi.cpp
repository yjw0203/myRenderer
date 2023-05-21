#include "yjw_vulkan_rhi.h"
#include "vulkan/vulkan.h"
#include "yjw_vulkan_shader_manager.h"
#include "yjw_vulkan_resource_manager.h"
#include "yjw_vulkan_resource_view_manager.h"
#include "yjw_vulkan_god_objects_manager.h"
#include "yjw_vulkan_command_buffer_manager.h" 
#include "yjw_vulkan_type_conversion.h" 
#include "yjw_vulkan_pipeline.h"
#include "yjw_vulkan_descriptors_set.h"
#include "yjw_vulkan_attachments_set.h"

namespace rhi
{
    void IVulkanRHI::initialize(CreateInfo info)
    {
        vulkanGod.initialize(info);
    }

    void IVulkanRHI::beginFrame()
    {
        vulkanGod.beginFrame();
    }

    void IVulkanRHI::endFrame(RHIResource* present_texture)
    {
        vulkanGod.endFrame(present_texture);
    }

    void IVulkanRHI::resourceBarrier(RHIResource* resource, RHIResourceState beforeState, RHIResourceState afterState)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().getCurrentCommandBuffer();
        transitionImageLayout(commandBuffer, *((VulkanResourceLocation*)resource->resourceLocation)->getVkImage(), ((VulkanResourceLocation*)resource->resourceLocation)->getDesc().imageInfo.format, VulkanConverter::convertResourceState(beforeState), VulkanConverter::convertResourceState(afterState));
    }

    void IVulkanRHI::resourceBarrierImmidiately(RHIResource* resource, RHIResourceState beforeState, RHIResourceState afterState)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().beginImmdiatelyCommandBuffer();
        transitionImageLayout(commandBuffer, *((VulkanResourceLocation*)resource->resourceLocation)->getVkImage(), ((VulkanResourceLocation*)resource->resourceLocation)->getDesc().imageInfo.format, VulkanConverter::convertResourceState(beforeState), VulkanConverter::convertResourceState(afterState));
        VulkanCommandBufferAllocater::Get().endImmdiatelyCommandBuffer(commandBuffer);
    }

    void IVulkanRHI::copyResourceImmidiately(RHIResource* src, RHIResource* dst)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().beginImmdiatelyCommandBuffer();
        VulkanResourceCopyer::copyResource(commandBuffer, src, dst);
        VulkanCommandBufferAllocater::Get().endImmdiatelyCommandBuffer(commandBuffer);
    }

    void IVulkanRHI::writeResourceImmidiately(RHIResource* resource, void* data, int size, int offset)
    {
        VulkanResourceWriter::writeResourceImmidiately(resource, data, size, offset);
    }

    void IVulkanRHI::clearImageResource(RHIResource* resource)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().getCurrentCommandBuffer();
        if (resource->getDesc().format == D24_unorm_S8_uint)
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
            vkCmdClearDepthStencilImage(commandBuffer, *((VulkanResourceLocation*)resource->resourceLocation)->getVkImage(), VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value, 1, &range);
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
            vkCmdClearColorImage(commandBuffer, *((VulkanResourceLocation*)resource->resourceLocation)->getVkImage(), VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value, 1, &range);
        }
    }

    void IVulkanRHI::beginPass(std::shared_ptr<RHIPipeline> pipeline, std::shared_ptr<RHIAttachmentsSet> attachmentsSet)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().getCurrentCommandBuffer();

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = ((VulkanPipelineLocation*)pipeline->location.get())->renderPass;
        renderPassInfo.framebuffer = ((VulkanAttachmentsSetLocation*)attachmentsSet->location.get())->frameBuffer;
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = vulkanGod.swapchainExtent;

        renderPassInfo.pClearValues = nullptr;

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ((VulkanPipelineLocation*)pipeline->location.get())->pipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = attachmentsSet->width;
        viewport.height = attachmentsSet->height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = vulkanGod.swapchainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void IVulkanRHI::endPass()
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().getCurrentCommandBuffer();
        vkCmdEndRenderPass(commandBuffer);
    }

    void IVulkanRHI::bindVertexBuffer(RHIResource* vertex_buffer)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().getCurrentCommandBuffer();
        VkDeviceSize offset = 0;
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, ((VulkanResourceLocation*)vertex_buffer->resourceLocation)->getVkBuffer(), &offset);
    }

    void IVulkanRHI::bindIndexBuffer(RHIResource* index_buffer)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().getCurrentCommandBuffer();
        vkCmdBindIndexBuffer(commandBuffer, *((VulkanResourceLocation*)index_buffer->resourceLocation)->getVkBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }

    void IVulkanRHI::bindDescriptorSet(const RHIDescriptorsSet* descriptorsSet)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().getCurrentCommandBuffer();
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ((VulkanPipelineLocation*)descriptorsSet->pipeline->location.get())->pipelineLayout, 0, ((VulkanDescriptorsSetLocation*)descriptorsSet->location.get())->descriptorSets.size(), ((VulkanDescriptorsSetLocation*)descriptorsSet->location.get())->descriptorSets.data(), 0, nullptr);
    }

    void IVulkanRHI::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().getCurrentCommandBuffer();
        vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void IVulkanRHI::drawIndex(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().getCurrentCommandBuffer();
        vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    std::shared_ptr<RHIPipelineLocation> IVulkanRHI::createPipeline(std::shared_ptr<RHIPipeline> pipeline)
    {
        return VulkanPipeline::createPipeline(pipeline);
    }

    void IVulkanRHI::destoryPipeline(std::shared_ptr<RHIPipelineLocation> location)
    {
        VulkanPipeline::destoryPipeline(location);
    }

    std::shared_ptr<RHIAttachmentsSetLocation> IVulkanRHI::createAttachmentsSet(std::shared_ptr<RHIAttachmentsSet> attachmentsSet)
    {
        return VulkanAttachmentsSet::createAttachmentsSet(attachmentsSet);
    }

    void IVulkanRHI::destoryAttachmentsSet(std::shared_ptr<RHIAttachmentsSetLocation> location)
    {
        VulkanAttachmentsSet::destoryAttachmentsSet(location);
    }

    std::shared_ptr<RHIDescriptorsSetLocation> IVulkanRHI::createDescriptorsSet(std::shared_ptr<RHIDescriptorsSet> descriptorsSet)
    {
        return VulkanDescritporsSet::createDescriptorsSet(descriptorsSet);
    }

    void IVulkanRHI::destoryDescriptorsSet(std::shared_ptr<RHIDescriptorsSetLocation> location)
    {
        VulkanDescritporsSet::destoryDescriptorsSet(location);
    }

    RHIResourceLocation* IVulkanRHI::createResource(const RHIResourceDesc& rhi_desc)
    {
        return VulkanResourceManager::Get().createResource(rhi_desc);
    }

    void IVulkanRHI::deleteResource(RHIResourceLocation*& location)
    {
        VulkanResourceManager::Get().deleteResource(location);
    }

    RHIResourceViewLocation* IVulkanRHI::createResourceView(ResourceViewType type, RHIResource* resource, RHIFormat view_format)
    {
        return VulkanResourceViewManager::Get().createResourceView(type, resource, view_format);
    }

    void IVulkanRHI::deleteResourceView(RHIResourceViewLocation*& location)
    {
        VulkanResourceViewManager::Get().deleteResourceView(location);
    }

    RHIShaderLocation* IVulkanRHI::createShader(const RHIShaderDesc& rhi_desc)
    {
        return VulkanShaderManager::Get().createShader(rhi_desc);
    }

    void IVulkanRHI::deleteShader(RHIShaderLocation*& location)
    {
        VulkanShaderManager::Get().deleteShader(location);
    }

}