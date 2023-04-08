#include "yjw_vulkan_draw_template_manager.h"
#include "yjw_vulkan_god_objects_manager.h"
#include "yjw_vulkan_command_buffer_manager.h"

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

        std::vector<VkPipelineShaderStageCreateInfo> shader_infos{ PipelineShaderStageCreateInfo(draw_template->getVertexShader(),VK_SHADER_STAGE_VERTEX_BIT),PipelineShaderStageCreateInfo(draw_template->getPixelShader(),VK_SHADER_STAGE_FRAGMENT_BIT)};

        buildCache->renderPass = createRenderPass(*draw_template->getRenderTargetViews(), draw_template->getDepthStencilView());
        buildCache->pipelineLayout = createPipelineLayout();
        buildCache->pipeline = createPipeline(buildCache->pipelineLayout, buildCache->renderPass, draw_template->getRasterizationState(), draw_template->getColorBlendState(), shader_infos);
        buildCache->frameBuffer = createFramebuffer(buildCache->renderPass, *draw_template->getRenderTargetViews(), draw_template->getDepthStencilView(), vulkanGod.swapchainExtent.width, vulkanGod.swapchainExtent.height);

        //record command buffer

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(buildCache->commandBuffer, &beginInfo);

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = buildCache->renderPass;
        renderPassInfo.framebuffer = buildCache->frameBuffer;
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = vulkanGod.swapchainExtent;

        VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;


        vkCmdBindPipeline(buildCache->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, buildCache->pipeline);

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
        vkCmdDraw(buildCache->commandBuffer, 3, 1, 0, 0);
        vkCmdEndRenderPass(buildCache->commandBuffer);

        vkEndCommandBuffer(buildCache->commandBuffer);
        return buildCache;
    }

    void VulkanDefaultDrawTemplateBuilder::deleteDrawTemplateCache(DefaultDrawTemplateBuildCache*& draw_template_cache)
    {
        VulkanDefaultDrawTemplateBuildCache* vulkanBuildCache = (VulkanDefaultDrawTemplateBuildCache*)draw_template_cache;

        VulkanCommandBufferAllocater::Get().free(vulkanBuildCache->commandBuffer);
        vkDestroyPipeline(vulkanGod.device, vulkanBuildCache->pipeline, nullptr);
        vkDestroyPipelineLayout(vulkanGod.device, vulkanBuildCache->pipelineLayout, nullptr);
        vkDestroyRenderPass(vulkanGod.device, vulkanBuildCache->renderPass, nullptr);
        vkDestroyFramebuffer(vulkanGod.device, vulkanBuildCache->frameBuffer, nullptr);

        delete draw_template_cache;
        draw_template_cache = nullptr;
    }
}