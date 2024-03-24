#include "yjw_vulkan_pipeline.h".
#include "yjw_vulkan_resource_cast.h"
#include "yjw_vulkan_type_conversation.h"

namespace rhi
{
    VulkanRenderPass::VulkanRenderPass(VulkanDevice* device)
        : VulkanDeviceObject(device)
    {
        VkRenderPassCreateInfo createInfo{};
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        if (m_native_render_pass)
        {
            vkDestroyRenderPass(GetDevice()->GetNativeDevice(), m_native_render_pass, nullptr);
        }
    }

    VkRenderPass VulkanRenderPass::GetNativeRenderPass()
    {
        return m_native_render_pass;
    }

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

        VkPipeline pipeline;

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

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};

        VkPipelineViewportStateCreateInfo viewportInfo{};

        VkPipelineRasterizationStateCreateInfo rasteriazationStateInfo{};

        VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo{};

        VkPipelineMultisampleStateCreateInfo multisampleStateInfo{};

        VkPipelineColorBlendStateCreateInfo colorBlendStateInfo{};

        VkPipelineDynamicStateCreateInfo dynamicStateInfo{};

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
        pipelineInfo.renderPass = nullptr;//renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        vkCreateGraphicsPipelines(GetDevice()->GetNativeDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);
    
        renderPass->retain(this);
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
    }

    VulkanRenderPipeline::~VulkanRenderPipeline()
    {
        m_descriptor.vs->release();
        m_descriptor.ps->release();
    }
    
}