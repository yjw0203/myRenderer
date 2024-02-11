#include "yjw_vulkan_pso.h"
#include "yjw_vulkan_global.h"
#include <stdexcept>
namespace vulkan
{
    int formatToSize(VkFormat format)
    {
        return 0;
    }

    void VulkanPSOCreation::bind(PSOShaderBinding* bind)
    {
        std::string entry(bind->entryName);
        VkShaderStageFlagBits stage = bind->shader->reflects[entry].stage;
        if (!stage)
        {
            throw std::runtime_error("can not find entry:" + entry + "in shader");
        }
        for (int i = 0; i <= stageCount; i++)
        {
            if (i == stageCount || shaderInfo[i].stage == stage)
            {
                shaderInfo[i].stage = stage;
                shaderInfo[i].module = bind->shader->shaderModule;
                shaderInfo[i].entryName = entry;
                if (i == stageCount)
                {
                    stageCount++;
                }
                break;
            }
        }
    }

    void VulkanPSOCreation::bind(PSOVertexBinding* bind)
    {
        int location_count = 0;

        int binding_count = bind->locations.size();

        vertexLayout_bindings.resize(binding_count);
        for (int iter = 0; iter < binding_count; iter++)
        {
            vertexLayout_bindings[iter].binding = iter;
            vertexLayout_bindings[iter].stride = 0;
            vertexLayout_bindings[iter].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            location_count += bind->locations[iter].size();
        }

        vertexLayout_attributes.resize(location_count);
        int current_iter = 0;
        for (int binding_itr = 0; binding_itr < binding_count; binding_itr++)
        {
            std::vector<VertexLocation>& binding = bind->locations[binding_itr];
            int current_offset = 0;
            for (int location_itr = 0; location_itr < binding.size(); location_itr++)
            {
                vertexLayout_attributes[current_iter].binding = binding_itr;
                vertexLayout_attributes[current_iter].format = binding[location_itr].format;
                vertexLayout_attributes[current_iter].location = binding[location_itr].location;
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

    void VulkanPSOCreation::bind(PSORasterizationStateBinding* bind)
    {
        pRasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        pRasterizationState.polygonMode = bind->polygonMode;
        pRasterizationState.lineWidth = bind->lineWidth;
        pRasterizationState.cullMode = bind->cullMode;
        pRasterizationState.frontFace = bind->frontFace;
        pRasterizationState.depthBiasEnable = VK_FALSE;
        pRasterizationState.depthClampEnable = VK_FALSE;
        pRasterizationState.rasterizerDiscardEnable = VK_FALSE;
    }

    void VulkanPSOCreation::bind(PSOAttachmentBinding* bind)
    {
        renderPass_attachments.resize(bind->color_attachments.size() + (bind->has_depth_stencil_attachment));
        for (int index = 0; index < bind->color_attachments.size(); index++)
        {
            renderPass_attachments[index].format = bind->color_attachments[index].format;
            renderPass_attachments[index].samples = VK_SAMPLE_COUNT_1_BIT;
            renderPass_attachments[index].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            renderPass_attachments[index].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            renderPass_attachments[index].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            renderPass_attachments[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            renderPass_attachments[index].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            renderPass_attachments[index].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }

        if (bind->has_depth_stencil_attachment)
        {
            renderPass_attachments.back().format = (bind->depth_stencil_attachment).format;
            renderPass_attachments.back().samples = VK_SAMPLE_COUNT_1_BIT;
            renderPass_attachments.back().loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            renderPass_attachments.back().storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            renderPass_attachments.back().stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            renderPass_attachments.back().stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            renderPass_attachments.back().initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            renderPass_attachments.back().finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }

        renderPass_attachmentRefs.resize(bind->color_attachments.size() + (bind->has_depth_stencil_attachment));
        for (int index = 0; index < bind->color_attachments.size(); index++)
        {
            renderPass_attachmentRefs[index].attachment = index;
            renderPass_attachmentRefs[index].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
        if (bind->has_depth_stencil_attachment)
        {
            renderPass_attachmentRefs.back().attachment = (int)renderPass_attachmentRefs.size() - 1;
            renderPass_attachmentRefs.back().layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }

        renderPass_subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        renderPass_subpass.colorAttachmentCount = bind->color_attachments.size();
        renderPass_subpass.pColorAttachments = renderPass_attachmentRefs.data();
        if (bind->has_depth_stencil_attachment)
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

    void VulkanPSOCreation::bind(PSODescriptorLayoutBinding* bind)
    {
        descriptorLayoutBinding = *bind;

        int set_count = bind->bindings.size();

        setLayout_bindings.resize(set_count);
        for (int set_index = 0; set_index < set_count; set_index++)
        {
            setLayout_bindings[set_index].resize(bind->bindings[set_index].size());
            for (int binding_index = 0; binding_index < bind->bindings[set_index].size(); binding_index++)
            {
                VkDescriptorSetLayoutBinding& layoutBinding = setLayout_bindings[set_index][binding_index];
                layoutBinding.binding = bind->bindings[set_index][binding_index].slot;
                layoutBinding.descriptorType = bind->bindings[set_index][binding_index].type;
                layoutBinding.stageFlags = bind->bindings[set_index][binding_index].shaderStage;
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

    VulkanPSO* DefaultVulkanPSOAllocateStrategy::CreateFunc(VulkanPSOCreation& creation)
    {
        VulkanPSO* pso = new VulkanPSO();

        if (vkCreateRenderPass(VK_G(VkDevice), &creation.renderPass_create_info, nullptr, &pso->renderPass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }

        pso->descriptorSetLayouts.resize(creation.sertLayout_createInfos.size());
        for (int i = 0; i < creation.sertLayout_createInfos.size(); i++)
        {
            vkCreateDescriptorSetLayout(VK_G(VkDevice), &creation.sertLayout_createInfos[i], nullptr, &pso->descriptorSetLayouts[i]);
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = pso->descriptorSetLayouts.size();
        pipelineLayoutInfo.pSetLayouts = pso->descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        if (vkCreatePipelineLayout(VK_G(VkDevice), &pipelineLayoutInfo, nullptr, &pso->pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }

        for (int stageIndex = 0; stageIndex < creation.stageCount; stageIndex++)
        {
            creation.pStages[stageIndex].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            creation.pStages[stageIndex].pNext = nullptr;
            creation.pStages[stageIndex].flags = 0;
            creation.pStages[stageIndex].stage = creation.shaderInfo[stageIndex].stage;
            creation.pStages[stageIndex].module = creation.shaderInfo[stageIndex].module;
            creation.pStages[stageIndex].pName = creation.shaderInfo[stageIndex].entryName.c_str();
            creation.pStages[stageIndex].pSpecializationInfo = nullptr;
        }

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = creation.stageCount;
        pipelineInfo.pStages = creation.pStages;
        pipelineInfo.pVertexInputState = &creation.pVertexInputState;
        pipelineInfo.pInputAssemblyState = &creation.pInputAssemblyState;
        pipelineInfo.pViewportState = &creation.pViewportState;
        pipelineInfo.pRasterizationState = &creation.pRasterizationState;
        pipelineInfo.pDepthStencilState = &creation.pDepthStencilState;
        pipelineInfo.pMultisampleState = &creation.pMultisampleState;
        pipelineInfo.pColorBlendState = &creation.pColorBlendState;
        pipelineInfo.pDynamicState = &creation.pDynamicState;
        pipelineInfo.layout = pso->pipelineLayout;
        pipelineInfo.renderPass = pso->renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(VK_G(VkDevice), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pso->pso) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        pso->descriptorLayoutBinding = creation.descriptorLayoutBinding;

        pso->attachmentCount = creation.renderPass_create_info.attachmentCount;

        return pso;
    }

    void DefaultVulkanPSOAllocateStrategy::DestoryFunc(VulkanPSO* resource)
    {
        vkDestroyPipeline(VK_G(VkDevice), resource->pso, nullptr);
        vkDestroyPipelineLayout(VK_G(VkDevice), resource->pipelineLayout, nullptr);
        for (VkDescriptorSetLayout& descriptorSetLayout : resource->descriptorSetLayouts)
        {
            vkDestroyDescriptorSetLayout(VK_G(VkDevice), descriptorSetLayout, nullptr);
        }
        vkDestroyRenderPass(VK_G(VkDevice), resource->renderPass, nullptr);
    }

    VulkanPSOHandle VulkanPSOPool::createPSO(VulkanPSOCreation& creation)
    {
        return DefaultAllocator.create(creation);
    }

    void VulkanPSOPool::destroyPSO(VulkanPSOHandle handle)
    {
        DefaultAllocator.destory(handle);
    }
}