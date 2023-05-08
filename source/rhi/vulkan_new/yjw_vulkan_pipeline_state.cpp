#include "yjw_vulkan_pipeline_state.h"
#include "yjw_vulkan_god_objects_manager.h"

namespace rhi
{
    void VulkanPipelineStateObject::build(VulkanPipelineDesc& desc)
    {
        std::vector<VkPipelineShaderStageCreateInfo> shader_infos{ PipelineShaderStageCreateInfo(desc.vs,VK_SHADER_STAGE_VERTEX_BIT),PipelineShaderStageCreateInfo(desc.ps,VK_SHADER_STAGE_FRAGMENT_BIT) };

        renderPass = createRenderPass(*desc.rtvs, desc.dsv);
        createDescriptorLayoutSets(descriptorPool, descriptorSetLayouts, descriptorSets, desc.vs, desc.ps);
        pipelineLayout = createPipelineLayout(descriptorSetLayouts);
        pipeline = createPipeline(pipelineLayout, desc.vertexLayout , renderPass, desc.rasterizationState, desc.depthStencilState, desc.colorBlendState, shader_infos);
        frameBuffer = createFramebuffer(renderPass, *desc.rtvs, desc.dsv, vulkanGod.swapchainExtent.width, vulkanGod.swapchainExtent.height);

    }

    void VulkanPipelineStateObject::clear()
    {
        vkDestroyPipeline(vulkanGod.device, pipeline, nullptr);
        vkDestroyPipelineLayout(vulkanGod.device, pipelineLayout, nullptr);
        for (VkDescriptorSetLayout descriptorSetLayout : descriptorSetLayouts)
        {
            vkDestroyDescriptorSetLayout(vulkanGod.device, descriptorSetLayout, nullptr);
        }
        descriptorSetLayouts.clear();
        vkDestroyRenderPass(vulkanGod.device, renderPass, nullptr);
        vkDestroyDescriptorPool(vulkanGod.device, descriptorPool, nullptr);
        descriptorSets.clear();
        vkDestroyFramebuffer(vulkanGod.device, frameBuffer, nullptr);
    }

    VkPipeline& VulkanPipelineStateObject::getVkPipeline()
    {
        return pipeline;
    }

    VkFramebuffer& VulkanPipelineStateObject::getVkFramebuffer()
    {
        return frameBuffer;
    }

    VkRenderPass& VulkanPipelineStateObject::getVkRenderPass()
    {
        return renderPass;
    }

    VkPipelineLayout& VulkanPipelineStateObject::getVkPipelineLayout()
    {
        return pipelineLayout;
    }
    
    std::vector<VkDescriptorSet>& VulkanPipelineStateObject::getVkDescriptorSets()
    {
        return descriptorSets;
    }
}