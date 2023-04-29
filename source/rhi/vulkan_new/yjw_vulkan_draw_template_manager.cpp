#include "yjw_vulkan_draw_template_manager.h"
#include "yjw_vulkan_god_objects_manager.h"
#include "yjw_vulkan_command_buffer_manager.h"
#include "yjw_vulkan_resource_manager.h"

#include <stdexcept>

namespace rhi
{
    void VulkanDefaultDrawTemplateBuilder::excuteDrawTemplate(const DefaultDrawTemplate* draw_template)
    {
        VulkanDefaultDrawTemplateBuildCache* buildCache = (VulkanDefaultDrawTemplateBuildCache*)draw_template->cache;
        vulkanGod.commandBufferList.push_back(buildCache->commandBuffer);
    }

    DefaultDrawTemplateBuildCache* VulkanDefaultDrawTemplateBuilder::buildDrawTemplateCache(DefaultDrawTemplate* draw_template)
    {
        VulkanDefaultDrawTemplateBuildCache* buildCache = new VulkanDefaultDrawTemplateBuildCache();

        // build objects
        VulkanCommandBufferAllocater::Get().allocate(buildCache->commandBuffer);

        VulkanPipelineDesc pipelineDesc{};
        pipelineDesc.rtvs = draw_template->getRenderTargetViews();
        pipelineDesc.dsv = draw_template->getDepthStencilView();
        pipelineDesc.rasterizationState = draw_template->getRasterizationState();
        pipelineDesc.colorBlendState = draw_template->getColorBlendState();
        pipelineDesc.vs = draw_template->getVertexShaderView();
        pipelineDesc.ps = draw_template->getPixelShaderView();
        pipelineDesc.vertexLayout = draw_template->getVertexLayout();

        buildCache->pso.build(pipelineDesc);

        //record command buffer

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(buildCache->commandBuffer, &beginInfo);

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = buildCache->pso.getVkRenderPass();
        renderPassInfo.framebuffer = buildCache->pso.getVkFramebuffer();
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = vulkanGod.swapchainExtent;

        VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBindPipeline(buildCache->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, buildCache->pso.getVkPipeline());

        if ((VulkanResourceLocation*)draw_template->getVertexBuffer())
        {
            VkDeviceSize offset = 0;
            vkCmdBindVertexBuffers(buildCache->commandBuffer, 0, 1, ((VulkanResourceLocation*)draw_template->getVertexBuffer()->resourceLocation)->getVkBuffer(), &offset);
        }

        if ((VulkanResourceLocation*)draw_template->getIndexBuffer())
        {
            vkCmdBindIndexBuffer(buildCache->commandBuffer, *((VulkanResourceLocation*)draw_template->getIndexBuffer()->resourceLocation)->getVkBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)vulkanGod.swapchainExtent.width;
        viewport.height = (float)vulkanGod.swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(buildCache->commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = vulkanGod.swapchainExtent;
        vkCmdSetScissor(buildCache->commandBuffer, 0, 1, &scissor);

        vkCmdBeginRenderPass(buildCache->commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindDescriptorSets(buildCache->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, buildCache->pso.getVkPipelineLayout(), 0, buildCache->pso.getVkDescriptorSets().size(), buildCache->pso.getVkDescriptorSets().data(), 0, nullptr);
        
        DrawOption& drawOption = draw_template->getDrawOption();
        if (drawOption.drawMode == DrawMode::none)
        {
        }
        else if (drawOption.drawMode == DrawMode::draw)
        {
            vkCmdDraw(buildCache->commandBuffer, drawOption.vertexCount, drawOption.instanceCount, drawOption.firstVertex, drawOption.firstInstance);
        }
        else if (drawOption.drawMode == DrawMode::draw_index)
        {
            vkCmdDrawIndexed(buildCache->commandBuffer, drawOption.indexCount, drawOption.instanceCount, drawOption.firstIndex, drawOption.vertexOffset, drawOption.firstInstance);
        }
        else
        {
            throw std::runtime_error("drawOption not support");
        }
        vkCmdEndRenderPass(buildCache->commandBuffer);

        vkEndCommandBuffer(buildCache->commandBuffer);
        return buildCache;
    }

    void VulkanDefaultDrawTemplateBuilder::deleteDrawTemplateCache(DefaultDrawTemplateBuildCache*& draw_template_cache)
    {
        VulkanDefaultDrawTemplateBuildCache* vulkanBuildCache = (VulkanDefaultDrawTemplateBuildCache*)draw_template_cache;

        VulkanCommandBufferAllocater::Get().free(vulkanBuildCache->commandBuffer);
        vulkanBuildCache->pso.clear();

        delete draw_template_cache;
        draw_template_cache = nullptr;
    }
}