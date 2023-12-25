#include "yjw_vulkan_pso.h"
#include "yjw_vulkan_global.h"
#include <stdexcept>
namespace vulkan
{
    int formatToSize(VkFormat format)
    {
        return 0;
    }

    void PSOFactory::bindShader(shader::Shader* shader, const char* entryName)
    {
        std::string entry(entryName);
        VkShaderStageFlagBits stage = shader->reflects[entry].stage;
        if (!stage)
        {
            throw std::runtime_error("can not find entry:" + entry + "in shader");
        }
        for (int i = 0; i <= stageCount; i++)
        {
            if (i == stageCount || shaderInfo[i].stage == stage)
            {
                shaderInfo[i].stage = stage;
                shaderInfo[i].module = shader->shaderModule;
                shaderInfo[i].entryName = entry;
                if (i == stageCount)
                {
                    stageCount++;
                }
                break;
            }
        }
    }

    void PSOFactory::bindVertexLayout(VertexBinding* bindings, int binding_count)
    {
        int location_count = 0;

        vertexLayout_bindings.resize(binding_count);
        for (int iter = 0; iter < binding_count; iter++)
        {
            vertexLayout_bindings[iter].binding = iter;
            vertexLayout_bindings[iter].stride = 0;
            vertexLayout_bindings[iter].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            location_count += bindings[iter].locations.size();
        }

        vertexLayout_attributes.resize(location_count);
        int current_iter = 0;
        for (int binding_itr = 0; binding_itr < binding_count; binding_itr++)
        {
            VertexBinding& binding = bindings[binding_itr];
            int current_offset = 0;
            for (int location_itr = 0; location_itr < binding.locations.size(); location_itr++)
            {
                vertexLayout_attributes[current_iter].binding = binding_itr;
                vertexLayout_attributes[current_iter].format = binding.locations[location_itr].format;
                vertexLayout_attributes[current_iter].location = binding.locations[location_itr].location;
                vertexLayout_attributes[current_iter].offset = current_offset;
                current_offset += formatToSize(vertexLayout_attributes[current_iter].format);
                current_iter++;
            }
        }

        pVertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        pVertexInputState.vertexBindingDescriptionCount = vertexLayout_bindings.size();
        pVertexInputState.pVertexBindingDescriptions = vertexLayout_bindings.data();
        pVertexInputState.vertexAttributeDescriptionCount = vertexLayout_attributes.size();
        pVertexInputState.pVertexAttributeDescriptions = vertexLayout_attributes.data();
    }

    void PSOFactory::bindRasterizationState(VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace,float lineWidth)
    {
        pRasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        pRasterizationState.polygonMode = polygonMode;
        pRasterizationState.lineWidth = lineWidth;
        pRasterizationState.cullMode = cullMode;
        pRasterizationState.frontFace = frontFace;
        pRasterizationState.depthBiasEnable = VK_FALSE;
        pRasterizationState.depthClampEnable = VK_FALSE;
        pRasterizationState.rasterizerDiscardEnable = VK_FALSE;
    }

    void PSOFactory::bindAttachments(AttachmentBinding* color_attachments, int color_attachments_count, AttachmentBinding* depth_stencil_attachment)
    {
        renderPass_attachments.resize(color_attachments_count + (depth_stencil_attachment != nullptr));
        for (int index = 0; index < color_attachments_count; index++)
        {
            renderPass_attachments[index].format = color_attachments[index].format;
            renderPass_attachments[index].samples = VK_SAMPLE_COUNT_1_BIT;
            renderPass_attachments[index].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            renderPass_attachments[index].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            renderPass_attachments[index].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            renderPass_attachments[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            renderPass_attachments[index].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            renderPass_attachments[index].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }

        if (depth_stencil_attachment)
        {
            renderPass_attachments.back().format = (*depth_stencil_attachment).format;
            renderPass_attachments.back().samples = VK_SAMPLE_COUNT_1_BIT;
            renderPass_attachments.back().loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            renderPass_attachments.back().storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            renderPass_attachments.back().stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            renderPass_attachments.back().stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            renderPass_attachments.back().initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            renderPass_attachments.back().finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }

        renderPass_attachmentRefs.resize(color_attachments_count + (depth_stencil_attachment != nullptr));
        for (int index = 0; index < color_attachments_count; index++)
        {
            renderPass_attachmentRefs[index].attachment = index;
            renderPass_attachmentRefs[index].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
        if (depth_stencil_attachment)
        {
            renderPass_attachmentRefs.back().attachment = (int)renderPass_attachmentRefs.size() - 1;
            renderPass_attachmentRefs.back().layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }
        
        renderPass_subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        renderPass_subpass.colorAttachmentCount = color_attachments_count;
        renderPass_subpass.pColorAttachments = renderPass_attachmentRefs.data();
        if (depth_stencil_attachment)
        {
            renderPass_subpass.pDepthStencilAttachment = &renderPass_attachmentRefs.back();
        }

        renderPass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        renderPass_dependency.dstSubpass = 0;
        renderPass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        renderPass_dependency.srcAccessMask = 0;
        renderPass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        renderPass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        renderPass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPass_create_info.attachmentCount = renderPass_attachments.size();
        renderPass_create_info.pAttachments = renderPass_attachments.data();
        renderPass_create_info.subpassCount = 1;
        renderPass_create_info.pSubpasses = &renderPass_subpass;
        renderPass_create_info.dependencyCount = 1;
        renderPass_create_info.pDependencies = &renderPass_dependency;
    }

    void PSOFactory::bindDescriptorSetLayouts(DescriptorSetLayout* descriptor_set_layouts, int set_count)
    {
        setLayout_bindings.resize(set_count);
        for (int set_index = 0; set_index < set_count; set_index++)
        {
            setLayout_bindings[set_index].resize(descriptor_set_layouts[set_index].bindings.size());
            for (int binding_index = 0; binding_index < descriptor_set_layouts[set_index].bindings.size(); binding_index++)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = setLayout_bindings[set_index][binding_index];
                layoutBinding.binding = descriptor_set_layouts[set_index].bindings[binding_index].slot;
                layoutBinding.descriptorType = descriptor_set_layouts[set_index].bindings[binding_index].type;
                layoutBinding.stageFlags = descriptor_set_layouts[set_index].bindings[binding_index].shaderStage;
                layoutBinding.descriptorCount = 1;
                layoutBinding.pImmutableSamplers = nullptr;
            }
        }
        sertLayout_createInfos.resize(set_count);
        for (int i = 0; i < set_count; i++)
        {
            VkDescriptorSetLayoutCreateInfo& layoutInfo = sertLayout_createInfos[i];
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = setLayout_bindings[i].size();
            layoutInfo.pBindings = setLayout_bindings[i].data();
        }

    }

	PSO PSOFactory::createGraphicsPSO()
	{
        PSO pso;

        if (vkCreateRenderPass(VK_G(VkDevice), &renderPass_create_info, nullptr, &pso.renderPass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }

        pso.descriptorSetLayouts.resize(sertLayout_createInfos.size());
        for (int i = 0; i < sertLayout_createInfos.size(); i++)
        {
            vkCreateDescriptorSetLayout(VK_G(VkDevice), &sertLayout_createInfos[i], nullptr, &pso.descriptorSetLayouts[i]);
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = pso.descriptorSetLayouts.size();
        pipelineLayoutInfo.pSetLayouts = pso.descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        if (vkCreatePipelineLayout(VK_G(VkDevice), &pipelineLayoutInfo, nullptr, &pso.pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }

        for (int stageIndex = 0; stageIndex < stageCount; stageIndex++)
        {
            pStages[stageIndex].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            pStages[stageIndex].pNext = nullptr;
            pStages[stageIndex].flags = 0;
            pStages[stageIndex].stage = shaderInfo[stageIndex].stage;
            pStages[stageIndex].module = shaderInfo[stageIndex].module;
            pStages[stageIndex].pName = shaderInfo[stageIndex].entryName.c_str();
            pStages[stageIndex].pSpecializationInfo = nullptr;
        }

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = stageCount;
        pipelineInfo.pStages = pStages;
        pipelineInfo.pVertexInputState = &pVertexInputState;
        pipelineInfo.pInputAssemblyState = &pInputAssemblyState;
        pipelineInfo.pViewportState = &pViewportState;
        pipelineInfo.pRasterizationState = &pRasterizationState;
        pipelineInfo.pDepthStencilState = &pDepthStencilState;
        pipelineInfo.pMultisampleState = &pMultisampleState;
        pipelineInfo.pColorBlendState = &pColorBlendState;
        pipelineInfo.pDynamicState = &pDynamicState;
        pipelineInfo.layout = pso.pipelineLayout;
        pipelineInfo.renderPass = pso.renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(VK_G(VkDevice), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pso.pso) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }
        return pso;
	}
}