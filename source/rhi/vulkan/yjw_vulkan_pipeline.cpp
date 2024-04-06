#include "yjw_vulkan_pipeline.h".
#include "yjw_vulkan_resource_cast.h"
#include "yjw_vulkan_type_conversation.h"

namespace rhi
{
    void VulkanResourceLayoutView::AddReflectionTable(VulkanShaderType shaderType, std::unordered_map<RHIName, VulkanResourceBinding>& reflection_table)
    {
        VkShaderStageFlagBits shaderBits = ConvertShaderTypeToVkStage(shaderType);
        for (auto& iter : reflection_table)
        {
            VulkanResourceBinding& binding = iter.second;
            VkDescriptorSetLayoutBinding decriptorBinding{};
            decriptorBinding.binding = binding.binding;
            decriptorBinding.descriptorCount = 1;
            decriptorBinding.stageFlags = shaderBits;
            decriptorBinding.pImmutableSamplers = nullptr;
            decriptorBinding.descriptorType = ConvertShaderResourceTypeToDescriptorType(binding.resourceType);
            m_sets[binding.setId].push_back(decriptorBinding);
            m_max_set_count = std::max(m_max_set_count, binding.setId);
        }
    }

    std::vector<VkDescriptorSetLayoutBinding>& VulkanResourceLayoutView::GetBindingsBySetID(int setId)
    {
        return m_sets[setId];
    }

    int VulkanResourceLayoutView::GetMaxSetCount()
    {
        return m_max_set_count;
    }

    VulkanRenderPipeline::VulkanRenderPipeline(VulkanDevice* device, const RHIRenderPipelineDescriptor& desc)
        : RHIRenderPipeline(desc), VulkanDeviceObject(device)
    {
        desc.vs->retain(this);
        desc.ps->retain(this);
    }

    VkPipeline VulkanRenderPipeline::GetOrCreateVkPipeline(VulkanRenderPass* renderPass)
    {
        auto find_iter = m_pipelines.find(renderPass);
        if (find_iter != m_pipelines.end())
        {
            return find_iter->second;
        }

        VkPipeline pipeline = nullptr;

        VkPipelineShaderStageCreateInfo shaderStages[2] = {};
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].module = ResourceCast(m_descriptor.vs)->GetNativeShaderModule();
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].pName = m_descriptor.vs_entry.c_str();
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].module = ResourceCast(m_descriptor.ps)->GetNativeShaderModule();
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].pName = m_descriptor.ps_entry.c_str();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.pNext = nullptr;
        vertexInputInfo.flags = 0;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
        inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
        inputAssemblyInfo.flags = 0;
        inputAssemblyInfo.pNext = nullptr;

        VkPipelineViewportStateCreateInfo viewportInfo{};
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = nullptr;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = nullptr;
        viewportInfo.flags = 0;
        viewportInfo.pNext = nullptr;

        VkPipelineRasterizationStateCreateInfo rasteriazationStateInfo{};
        rasteriazationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasteriazationStateInfo.depthClampEnable = VK_FALSE;
        rasteriazationStateInfo.rasterizerDiscardEnable = VK_FALSE;
        rasteriazationStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        rasteriazationStateInfo.lineWidth = 1.0f;
        rasteriazationStateInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
        rasteriazationStateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasteriazationStateInfo.depthBiasEnable = VK_FALSE;
        rasteriazationStateInfo.depthBiasClamp = 0.0;
        rasteriazationStateInfo.flags = 0;
        rasteriazationStateInfo.pNext = nullptr;

        VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo{};
        depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilStateInfo.depthTestEnable = VK_FALSE;
        depthStencilStateInfo.depthWriteEnable = VK_FALSE;
        depthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
        depthStencilStateInfo.stencilTestEnable = VK_FALSE;
        depthStencilStateInfo.front = VkStencilOpState();
        depthStencilStateInfo.back = VkStencilOpState();
        depthStencilStateInfo.minDepthBounds = 0;
        depthStencilStateInfo.maxDepthBounds = 0;
        depthStencilStateInfo.flags = 0;
        depthStencilStateInfo.pNext = nullptr;

        VkPipelineMultisampleStateCreateInfo multisampleStateInfo{};
        multisampleStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleStateInfo.sampleShadingEnable = VK_FALSE;
        multisampleStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleStateInfo.pSampleMask = nullptr;
        multisampleStateInfo.alphaToCoverageEnable = VK_FALSE;
        multisampleStateInfo.alphaToOneEnable = VK_FALSE;
        multisampleStateInfo.flags = 0;
        multisampleStateInfo.pNext = nullptr;

        VkPipelineColorBlendStateCreateInfo colorBlendStateInfo{};
        VkPipelineColorBlendAttachmentState colorBlendStates[RHI_MAX_RENDER_TARGETS];
        for (int index = 0; index < renderPass->GetDesc().colorAttachmentCount; index++)
        {
            colorBlendStates[index].colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT;
            colorBlendStates[index].blendEnable = VK_FALSE;
            colorBlendStates[index].srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
            colorBlendStates[index].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
            colorBlendStates[index].colorBlendOp = VK_BLEND_OP_ADD; // Optional
            colorBlendStates[index].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
            colorBlendStates[index].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
            colorBlendStates[index].alphaBlendOp = VK_BLEND_OP_ADD; // Optional
        }

        colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendStateInfo.pNext = nullptr;
        colorBlendStateInfo.logicOpEnable = VK_FALSE;
        colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
        colorBlendStateInfo.attachmentCount = renderPass->GetDesc().colorAttachmentCount;
        colorBlendStateInfo.pAttachments = colorBlendStates;

        VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
        static std::vector<VkDynamicState> dynamicStates = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR
        };
        dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicStateInfo.pDynamicStates = dynamicStates.data();
        dynamicStateInfo.flags = 0;
        dynamicStateInfo.pNext = nullptr;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &rasteriazationStateInfo;
        pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
        pipelineInfo.pMultisampleState = &multisampleStateInfo;
        pipelineInfo.pColorBlendState = &colorBlendStateInfo;
        pipelineInfo.pDynamicState = &dynamicStateInfo;
        pipelineInfo.layout = GetOrCreateVkPipelineLayout();
        pipelineInfo.renderPass = renderPass->GetNativeRenderPass();
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        vkCreateGraphicsPipelines(GetDevice()->GetNativeDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);
    
        renderPass->retain(this);
        m_pipelines[renderPass] = pipeline;
        return pipeline;
    }

    VkPipelineLayout VulkanRenderPipeline::GetOrCreateVkPipelineLayout()
    {
        if (m_pipeline_layout)
        {
            return m_pipeline_layout;
        }

        VulkanResourceLayoutView layoutView;
        layoutView.AddReflectionTable(VulkanShaderType::vertex, ResourceCast(m_descriptor.vs)->GetReflectionTableByEntryName(RHIName(m_descriptor.vs_entry)));
        layoutView.AddReflectionTable(VulkanShaderType::vertex, ResourceCast(m_descriptor.ps)->GetReflectionTableByEntryName(RHIName(m_descriptor.ps_entry)));
        int descriptorSetCount = layoutView.GetMaxSetCount() + 1;
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        descriptorSetLayouts.resize(descriptorSetCount);
        for (int setId = 0; setId < descriptorSetCount; setId++)
        {
            std::vector<VkDescriptorSetLayoutBinding>& bindings = layoutView.GetBindingsBySetID(setId);
            VkDescriptorSetLayoutCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            createInfo.bindingCount = bindings.size();
            createInfo.pBindings = bindings.data();
            createInfo.flags = 0;
            createInfo.pNext = nullptr;
            vkCreateDescriptorSetLayout(GetDevice()->GetNativeDevice(), &createInfo, nullptr, &descriptorSetLayouts[setId]);
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo;
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pNext = nullptr;
        pipelineLayoutInfo.flags = 0;
        pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        vkCreatePipelineLayout(GetDevice()->GetNativeDevice(), &pipelineLayoutInfo, nullptr, &m_pipeline_layout);

        for (int setId = 0; setId < descriptorSetCount; setId++)
        {
            vkDestroyDescriptorSetLayout(GetDevice()->GetNativeDevice(), descriptorSetLayouts[setId], nullptr);
        }

        return m_pipeline_layout;
    }

    VulkanRenderPipeline::~VulkanRenderPipeline()
    {
        m_descriptor.vs->release();
        m_descriptor.ps->release();
        for (auto iter : m_pipelines)
        {
            VulkanRenderPass* renderPass = iter.first;
            renderPass->release();
        }
    }
    
}