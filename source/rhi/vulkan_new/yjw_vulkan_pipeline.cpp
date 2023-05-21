#include "yjw_vulkan_pipeline.h"
#include "yjw_vulkan_god_objects_manager.h"
#include "yjw_vulkan_type_conversion.h"
#include "yjw_vulkan_shader_manager.h"
#include "yjw_vulkan_pipeline_state.h"
#include <stdexcept>

namespace rhi
{
    std::shared_ptr<RHIPipelineLocation> VulkanPipeline::createPipeline(std::shared_ptr<RHIPipeline> pipeline)
    {
        std::shared_ptr<VulkanPipelineLocation> vulkanLocation = std::make_shared<VulkanPipelineLocation>();
        std::shared_ptr<RHIPipelineLocation> rhiLocation = vulkanLocation;
        
        //vertex layout
        std::vector<VkVertexInputBindingDescription> vextexBindings(pipeline->num_vertex_binding);
        for (int iter = 0; iter < vextexBindings.size(); iter++)
        {
            vextexBindings[iter].binding = iter;
            vextexBindings[iter].stride = 0;
            vextexBindings[iter].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        }

        std::vector<VkVertexInputAttributeDescription> vertexAttributes(pipeline->vertex_bindings.size());
        int current_iter = 0;
        for (auto& binding : pipeline->vertex_bindings)
        {
            vextexBindings[binding.first.first].stride += sizeofFormat(binding.second);

            vertexAttributes[current_iter].binding = binding.first.first;
            vertexAttributes[current_iter].format = VulkanConverter::convertFormat(binding.second);
            vertexAttributes[current_iter].location = binding.first.second;
            vertexAttributes[current_iter].offset = 0;
            for (auto& binding1 : pipeline->vertex_bindings)
            {
                if (binding1.first.first == binding.first.first && binding1.first.second < binding.first.second)
                {
                    vertexAttributes[current_iter].offset += sizeofFormat(binding1.second);
                }
            }
            current_iter++;
        }

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = vextexBindings.size();
        vertexInputInfo.pVertexBindingDescriptions = vextexBindings.data();
        vertexInputInfo.vertexAttributeDescriptionCount = vertexAttributes.size();
        vertexInputInfo.pVertexAttributeDescriptions = vertexAttributes.data();

        //descriptor set layout
        vulkanLocation->descriptorSetLayouts.resize(pipeline->num_set);
        std::vector<std::vector<VkDescriptorSetLayoutBinding>> setLayoutBindings(pipeline->num_set);
        current_iter = 0;
        for (int set_id = 0; set_id < setLayoutBindings.size(); set_id++)
        {
            int binding_count = 0;
            for (int stage = 0; stage < RHIShaderType::shader_type_count; stage++)
            {
                for (auto& binding : pipeline->uniform_buffer[stage])
                {
                    if (binding.first.first == set_id)
                    {
                        binding_count++;
                    }
                }
                for (auto& binding : pipeline->texture_srv[stage])
                {
                    if (binding.first == set_id)
                    {
                        binding_count++;
                    }
                }
            }
            setLayoutBindings[set_id].resize(binding_count);
        }

        std::vector<int> current_binding_id(pipeline->num_set);
        for (int stage = 0; stage < RHIShaderType::shader_type_count; stage++)
        {
            for (auto& binding : pipeline->uniform_buffer[stage])
            {
                VkDescriptorSetLayoutBinding& layoutBinding = setLayoutBindings[binding.first.first][current_binding_id[binding.first.first]];
                layoutBinding.binding = binding.first.second;
                layoutBinding.descriptorCount = 1;
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.stageFlags = VulkanConverter::convertShaderStageFlag((RHIShaderType)stage);

                current_binding_id[binding.first.first]++;
            }
            for (auto& binding : pipeline->texture_srv[stage])
            {
                VkDescriptorSetLayoutBinding& layoutBinding = setLayoutBindings[binding.first][current_binding_id[binding.first]];
                layoutBinding.binding = binding.second;
                layoutBinding.descriptorCount = 1;
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.stageFlags = VulkanConverter::convertShaderStageFlag((RHIShaderType)stage);

                current_binding_id[binding.first]++;
            }
        }

        for (int i = 0; i < pipeline->num_set; i++)
        {
            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = setLayoutBindings[i].size();
            layoutInfo.pBindings = setLayoutBindings[i].data();

            vkCreateDescriptorSetLayout(vulkanGod.device, &layoutInfo, nullptr, &vulkanLocation->descriptorSetLayouts[i]);
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = vulkanLocation->descriptorSetLayouts.size();
        pipelineLayoutInfo.pSetLayouts = vulkanLocation->descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        if (vkCreatePipelineLayout(vulkanGod.device, &pipelineLayoutInfo, nullptr, &vulkanLocation->pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }

        // render pass
        std::vector <VkAttachmentDescription> attachments(pipeline->num_color_attachment + pipeline->has_depth_stencil_attachment);
        for (auto& attachment : pipeline->color_attachments)
        {
            attachments[attachment.first].format = VulkanConverter::convertFormat(attachment.second);
            attachments[attachment.first].samples = VK_SAMPLE_COUNT_1_BIT;
            attachments[attachment.first].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            attachments[attachment.first].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments[attachment.first].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachments[attachment.first].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachments[attachment.first].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachments[attachment.first].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }

        if (pipeline->has_depth_stencil_attachment)
        {
            attachments.back().format = VulkanConverter::convertFormat(pipeline->depth_stencil_attachment);
            attachments.back().samples = VK_SAMPLE_COUNT_1_BIT;
            attachments.back().loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            attachments.back().storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments.back().stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachments.back().stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachments.back().initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachments.back().finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }

        std::vector<VkAttachmentReference>attachmentRefs(pipeline->num_color_attachment + pipeline->has_depth_stencil_attachment);
        for (int i = 0; i < pipeline->num_color_attachment; i++)
        {
            attachmentRefs[i].attachment = i;
            attachmentRefs[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
        if (pipeline->has_depth_stencil_attachment)
        {
            attachmentRefs.back().attachment = (int)attachmentRefs.size() - 1;
            attachmentRefs.back().layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = pipeline->num_color_attachment;
        subpass.pColorAttachments = attachmentRefs.data();
        if (pipeline->has_depth_stencil_attachment)
        {
            subpass.pDepthStencilAttachment = &attachmentRefs.back();
        }

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = attachments.size();
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(vulkanGod.device, &renderPassInfo, nullptr, &vulkanLocation->renderPass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }

        //shader
        int num_shader_stage = 0;
        VulkanShaderLocation* vs = pipeline->vs? num_shader_stage++, (VulkanShaderLocation*)pipeline->vs->shaderLocation : nullptr;
        VulkanShaderLocation* ps = pipeline->ps? num_shader_stage++, (VulkanShaderLocation*)pipeline->ps->shaderLocation : nullptr;
        VulkanShaderLocation* cs = pipeline->cs ? num_shader_stage++, (VulkanShaderLocation*)pipeline->cs->shaderLocation : nullptr;

        std::vector<VkPipelineShaderStageCreateInfo> shaderStages(num_shader_stage);
        int current_stage = 0;
        if (vs)
        {
            shaderStages[current_stage].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStages[current_stage].stage = VK_SHADER_STAGE_VERTEX_BIT;
            shaderStages[current_stage].module = vs->getShaderModule();
            shaderStages[current_stage].pName = pipeline->vs_entry.c_str();
            current_stage++;
        }

        if (ps)
        {
            shaderStages[current_stage].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStages[current_stage].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            shaderStages[current_stage].module = ps->getShaderModule();
            shaderStages[current_stage].pName = pipeline->ps_entry.c_str();
            current_stage++;
        }

        if (cs)
        {
            shaderStages[current_stage].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStages[current_stage].stage = VK_SHADER_STAGE_COMPUTE_BIT;
            shaderStages[current_stage].module = cs->getShaderModule();
            shaderStages[current_stage].pName = pipeline->cs_entry.c_str();
            current_stage++;
        }

        //other
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();


        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &VulkanPilelineStateManager::Get().getRasterizationState(pipeline->rasterizationState);
        pipelineInfo.pDepthStencilState = &VulkanPilelineStateManager::Get().getDepthStencilState(pipeline->depthStencilState);
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &VulkanPilelineStateManager::Get().getColorBlendState(pipeline->colorBlendState);
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = vulkanLocation->pipelineLayout;
        pipelineInfo.renderPass = vulkanLocation->renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(vulkanGod.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vulkanLocation->pipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        return rhiLocation;
    }

    void VulkanPipeline::destoryPipeline(std::shared_ptr<RHIPipelineLocation> location)
    {
        VulkanPipelineLocation* vulkanLocation = (VulkanPipelineLocation*)location.get();
        vkDestroyPipeline(vulkanGod.device, vulkanLocation->pipeline, nullptr);
        vkDestroyPipelineLayout(vulkanGod.device, vulkanLocation->pipelineLayout, nullptr);
        for (VkDescriptorSetLayout& descriptorSetLayout : vulkanLocation->descriptorSetLayouts)
        {
            vkDestroyDescriptorSetLayout(vulkanGod.device, descriptorSetLayout, nullptr);
        }
        vkDestroyRenderPass(vulkanGod.device, vulkanLocation->renderPass, nullptr);
    }
}