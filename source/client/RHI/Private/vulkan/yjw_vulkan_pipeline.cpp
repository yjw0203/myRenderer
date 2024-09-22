#include "Private/vulkan/yjw_vulkan_pipeline.h".
#include "Private/vulkan/yjw_vulkan_resource_cast.h"
#include "Private/vulkan/yjw_vulkan_type_conversation.h"
#include "Private/vulkan/yjw_vulkan_resource_binding.h"

namespace rhi
{
    void VulkanResourceLayoutView::AddReflectionTable(RHIShaderType shaderType, const ShaderReflect& reflect)
    {
        VkShaderStageFlagBits shaderBits = ConvertShaderTypeToVkStage(shaderType);
        m_reflect_table[(int)shaderType] = reflect;
        for (const ShaderReflect::UBO& ubo : reflect.m_ubos)
        {
            AddBinding(shaderBits,RHIName(ubo.m_name), ubo.m_set, ubo.m_binding, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
        }
        for (const ShaderReflect::SeparateImage& image : reflect.m_separate_images)
        {
            AddBinding(shaderBits, RHIName(image.m_name), image.m_set, image.m_binding, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
        }
        for (const ShaderReflect::SamplerBuffer& buffer : reflect.m_sampler_buffers)
        {
            AddBinding(shaderBits, RHIName(buffer.m_name), buffer.m_set, buffer.m_binding, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER);
        }
        if (shaderType == RHIShaderType::vertex)
        {
            for (const auto& input : reflect.m_inputs)
            {
                m_vertex_input_table[RHIName(input.m_name)] = input.m_loacation;
            }
        }
    }

    void VulkanResourceLayoutView::AddBinding(VkShaderStageFlagBits shaderBits, RHIName name, int set, int binding, VkDescriptorType descriptorType)
    {
        m_resource_table[(int)ConvertVkStageToShaderType(shaderBits)][name] = VariableBinding{ descriptorType ,set, binding };
        VkDescriptorSetLayoutBinding decriptorBinding{};
        decriptorBinding.binding = binding;
        decriptorBinding.descriptorCount = 1;
        decriptorBinding.stageFlags = shaderBits;
        decriptorBinding.pImmutableSamplers = nullptr;
        decriptorBinding.descriptorType = descriptorType;
        for (int i = 0; i <= m_sets[set].size(); i++)
        {
            if (i == m_sets[set].size())
            {
                //no exist binding
                m_sets[set].push_back(decriptorBinding);
                break;
            }
            else if (m_sets[set][i].binding == decriptorBinding.binding)
            {
                //exist binding
                m_sets[set][i].stageFlags |= shaderBits;
                break;
            }
        }
        m_max_set_id = std::max(m_max_set_id, set);
    }

    std::vector<VkDescriptorSetLayoutBinding>& VulkanResourceLayoutView::GetBindingsBySetID(int setId)
    {
        return m_sets[setId];
    }

    int VulkanResourceLayoutView::GetMaxSetCount()
    {
        return m_max_set_id + 1;
    }

    int VulkanResourceLayoutView::GetDescriptorCount(VkDescriptorType type)
    {
        int count = 0;
        for (int setId = 0; setId <= m_max_set_id; setId++)
        {
            for (VkDescriptorSetLayoutBinding& binding : m_sets[setId])
            {
                if (binding.descriptorType == type)
                {
                    count++;
                }
            }
        }
        return count;
    }

    int VulkanResourceLayoutView::GetVertexBindingCount()
    {
        return m_reflect_table[(int)RHIShaderType::vertex].m_inputs.size();
    }

    const VulkanResourceLayoutView::VariableBinding* VulkanResourceLayoutView::GetVariableBinding(RHIShaderType shaderType, RHIName name)
    {
        if (m_resource_table[(int)shaderType].find(name) == m_resource_table[(int)shaderType].end())
        {
            return nullptr;
        }
        return &m_resource_table[(int)shaderType][name];
    }

    int VulkanResourceLayoutView::GetVertexInputLocation(const RHIName& name)
    {
        if (m_vertex_input_table.find(name) == m_vertex_input_table.end())
        {
            return -1;
        }
        return m_vertex_input_table[name];
    }

    VulkanRenderPipeline::VulkanRenderPipeline(VulkanDevice* device, const RHIRenderPipelineDescriptor& desc)
        : RHIRenderPipeline(desc), VulkanDeviceObject(device)
    {
        desc.vs->retain(this);
        desc.ps->retain(this);
        m_reflect_view.AddReflectionTable(RHIShaderType::vertex, VKResourceCast(desc.vs)->GetReflect());
        m_reflect_view.AddReflectionTable(RHIShaderType::fragment, VKResourceCast(desc.ps)->GetReflect());
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
        shaderStages[0].module = VKResourceCast(m_descriptor.vs)->GetNativeShaderModule();
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].pName = VKResourceCast(m_descriptor.vs)->GetEntryName();
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].module = VKResourceCast(m_descriptor.ps)->GetNativeShaderModule();
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].pName = VKResourceCast(m_descriptor.ps)->GetEntryName();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        const std::vector<ShaderReflect::Input>& vertex_refect = VKResourceCast(m_descriptor.vs)->GetReflect().m_inputs;
        std::vector<VkVertexInputBindingDescription> inputBindingDescs;
        inputBindingDescs.resize(vertex_refect.size());
        std::vector<VkVertexInputAttributeDescription> inputAttributeDescs;
        inputAttributeDescs.resize(vertex_refect.size());
        int current_index = 0;
        for (auto& iter : vertex_refect)
        {
            inputBindingDescs[current_index].binding = current_index;
            inputBindingDescs[current_index].stride = ShaderReflect::DataTypeToSize(iter.m_type);
            inputBindingDescs[current_index].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            inputAttributeDescs[current_index].location = iter.m_loacation;
            inputAttributeDescs[current_index].binding = current_index;
            inputAttributeDescs[current_index].format = ConvertDataTypeToVkFormat(iter.m_type);
            inputAttributeDescs[current_index].offset = 0;
            current_index++;
        }

        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.pNext = nullptr;
        vertexInputInfo.flags = 0;
        vertexInputInfo.vertexBindingDescriptionCount = inputBindingDescs.size();
        vertexInputInfo.pVertexBindingDescriptions = inputBindingDescs.data();
        vertexInputInfo.vertexAttributeDescriptionCount = inputAttributeDescs.size();
        vertexInputInfo.pVertexAttributeDescriptions = inputAttributeDescs.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
        inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyInfo.topology = ConvertPrimitiveTopologyToVkPrimitiveTopology(m_descriptor.primitiveTopology);
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
        rasteriazationStateInfo.cullMode = ConvertCullModeToVkCullMode(m_descriptor.rasterization_state.cull_mode);
        rasteriazationStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasteriazationStateInfo.depthBiasEnable = VK_FALSE;
        rasteriazationStateInfo.depthBiasClamp = 0.0;
        rasteriazationStateInfo.flags = 0;
        rasteriazationStateInfo.pNext = nullptr;

        VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo{};
        depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilStateInfo.depthTestEnable = m_descriptor.depth_stencil_state.depthTestEnable;
        depthStencilStateInfo.depthWriteEnable = m_descriptor.depth_stencil_state.depthWriteEnable;
        depthStencilStateInfo.depthCompareOp = ConvertCompareOpToVkCompareOp(m_descriptor.depth_stencil_state.depthCompareOp);
        depthStencilStateInfo.depthBoundsTestEnable = m_descriptor.depth_stencil_state.depthBoundsTestEnable;
        depthStencilStateInfo.stencilTestEnable = m_descriptor.depth_stencil_state.stencilTestEnable;
        depthStencilStateInfo.front = ConvertStencilOpStateToVkStencilOpState(m_descriptor.depth_stencil_state.front);
        depthStencilStateInfo.back = ConvertStencilOpStateToVkStencilOpState(m_descriptor.depth_stencil_state.back);
        depthStencilStateInfo.minDepthBounds = m_descriptor.depth_stencil_state.minDepthBounds;
        depthStencilStateInfo.maxDepthBounds = m_descriptor.depth_stencil_state.maxDepthBounds;
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

        int descriptorSetCount = m_reflect_view.GetMaxSetCount();
        m_descriptor_set_layouts.resize(descriptorSetCount);
        for (int setId = 0; setId < descriptorSetCount; setId++)
        {
            std::vector<VkDescriptorSetLayoutBinding>& bindings = m_reflect_view.GetBindingsBySetID(setId);
            VkDescriptorSetLayoutCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            createInfo.bindingCount = bindings.size();
            createInfo.pBindings = bindings.data();
            createInfo.flags = 0;
            createInfo.pNext = nullptr;
            vkCreateDescriptorSetLayout(GetDevice()->GetNativeDevice(), &createInfo, nullptr, &m_descriptor_set_layouts[setId]);
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo;
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pNext = nullptr;
        pipelineLayoutInfo.flags = 0;
        pipelineLayoutInfo.setLayoutCount = m_descriptor_set_layouts.size();
        pipelineLayoutInfo.pSetLayouts = m_descriptor_set_layouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        vkCreatePipelineLayout(GetDevice()->GetNativeDevice(), &pipelineLayoutInfo, nullptr, &m_pipeline_layout);

        return m_pipeline_layout;
    }

    RHIResourceBinding* VulkanRenderPipeline::CreateResourceBinding()
    {
        GetOrCreateVkPipelineLayout();
        return new VulkanResourceBinding(GetDevice(), m_reflect_view, m_descriptor_set_layouts.data(), m_descriptor_set_layouts.size());
    }

    RHIPrimitiveBinding* VulkanRenderPipeline::CreatePrimitiveBinding()
    {
        return new VulkanPrimitiveBinding(GetDevice(), m_reflect_view);
    }

    ShaderReflect* VulkanRenderPipeline::GetVSShaderReflect()
    {
        return m_descriptor.vs->GetShaderReflect();
    }

    ShaderReflect* VulkanRenderPipeline::GetPSShaderReflect()
    {
        return m_descriptor.ps->GetShaderReflect();
    }

    VulkanRenderPipeline::~VulkanRenderPipeline()
    {
        m_descriptor.vs->release();
        m_descriptor.ps->release();
        for (auto iter : m_pipelines)
        {
            VulkanRenderPass* renderPass = iter.first;
            renderPass->release();
            vkDestroyPipeline(GetDevice()->GetNativeDevice(), iter.second, nullptr);
        }

        for (int setId = 0; setId < m_descriptor_set_layouts.size(); setId++)
        {
            vkDestroyDescriptorSetLayout(GetDevice()->GetNativeDevice(), m_descriptor_set_layouts[setId], nullptr);
        }

        vkDestroyPipelineLayout(GetDevice()->GetNativeDevice(), m_pipeline_layout, nullptr);

    }

    VulkanComputePipeline::VulkanComputePipeline(VulkanDevice* device, const RHIComputePipelineDescriptor& desc)
        : RHIComputePipeline(desc), VulkanDeviceObject(device)
    {
        desc.cs->retain(this);
        m_reflect_view.AddReflectionTable(RHIShaderType::compute, VKResourceCast(desc.cs)->GetReflect());
    }

    RHIResourceBinding* VulkanComputePipeline::CreateResourceBinding()
    {
        GetOrCreateVkPipelineLayout();
        return new VulkanResourceBinding(GetDevice(), m_reflect_view, m_descriptor_set_layouts.data(), m_descriptor_set_layouts.size());
    }

    VkPipeline VulkanComputePipeline::GetOrCreateVkPipeline()
    {
        if (m_pipeline)
        {
            return m_pipeline;
        }
        VkPipelineShaderStageCreateInfo shaderStageInfo{};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.module = VKResourceCast(m_descriptor.cs)->GetNativeShaderModule();
        shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStageInfo.pName = VKResourceCast(m_descriptor.cs)->GetEntryName();

        VkComputePipelineCreateInfo pipelineCreateInfo{};
        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.stage = shaderStageInfo;
        pipelineCreateInfo.basePipelineIndex = 0;
        pipelineCreateInfo.basePipelineHandle = nullptr;
        pipelineCreateInfo.layout = GetOrCreateVkPipelineLayout();
        pipelineCreateInfo.pNext = nullptr;
        pipelineCreateInfo.flags = 0;
        vkCreateComputePipelines(GetDevice()->GetNativeDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_pipeline);
        return m_pipeline;
    }

    VkPipelineLayout VulkanComputePipeline::GetOrCreateVkPipelineLayout()
    {
        if (m_pipeline_layout)
        {
            return m_pipeline_layout;
        }

        int descriptorSetCount = m_reflect_view.GetMaxSetCount();
        m_descriptor_set_layouts.resize(descriptorSetCount);
        for (int setId = 0; setId < descriptorSetCount; setId++)
        {
            std::vector<VkDescriptorSetLayoutBinding>& bindings = m_reflect_view.GetBindingsBySetID(setId);
            VkDescriptorSetLayoutCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            createInfo.bindingCount = bindings.size();
            createInfo.pBindings = bindings.data();
            createInfo.flags = 0;
            createInfo.pNext = nullptr;
            vkCreateDescriptorSetLayout(GetDevice()->GetNativeDevice(), &createInfo, nullptr, &m_descriptor_set_layouts[setId]);
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo;
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pNext = nullptr;
        pipelineLayoutInfo.flags = 0;
        pipelineLayoutInfo.setLayoutCount = m_descriptor_set_layouts.size();
        pipelineLayoutInfo.pSetLayouts = m_descriptor_set_layouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        vkCreatePipelineLayout(GetDevice()->GetNativeDevice(), &pipelineLayoutInfo, nullptr, &m_pipeline_layout);

        return m_pipeline_layout;
    }

    VulkanComputePipeline::~VulkanComputePipeline()
    {
        m_descriptor.cs->release();
        if (m_pipeline)
        {
            vkDestroyPipeline(GetDevice()->GetNativeDevice(), m_pipeline, nullptr);
            m_pipeline = nullptr;
        }

        for (int setId = 0; setId < m_descriptor_set_layouts.size(); setId++)
        {
            vkDestroyDescriptorSetLayout(GetDevice()->GetNativeDevice(), m_descriptor_set_layouts[setId], nullptr);
        }

        vkDestroyPipelineLayout(GetDevice()->GetNativeDevice(), m_pipeline_layout, nullptr);
    }
    
}