#pragma once
#include "vulkan/vulkan.h"
#include "rhi/vulkan/yjw_vulkan_default_states.h"
#include <vector>
#include "shader/yjw_shader_define.h"

namespace vulkan
{
    class AttachmentBinding
    {
    public:
        VkFormat format{};
    };

    class DescriptorSetLayoutBinding
    {
    public:
        DescriptorSetLayoutBinding(int in_slot, VkDescriptorType in_type,VkShaderStageFlags in_shaderStage):slot(in_slot),type(in_type),shaderStage(in_shaderStage)
        {}
        int slot{};
        VkDescriptorType type{};
        VkShaderStageFlags shaderStage{};
    };

    class DescriptorSetLayout
    {
    public:
        std::vector<DescriptorSetLayoutBinding> bindings;
    };

    class VertexLocation
    {
    public:
        int location{};
        VkFormat format{};
    };

    class VertexBinding
    {
    public:
        std::vector<VertexLocation> locations;
    };

    class ShaderCreateInfo
    {
    public:
        VkShaderStageFlagBits stage;
        VkShaderModule module;
        std::string entryName;
    };

    class PSO
    {
        friend class PSOFactory;
    private:
        VkRenderPass renderPass{};
        VkPipelineLayout pipelineLayout{};
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        VkPipeline pso{};
    };

	class PSOFactory
	{
	public:
        void bindShader(shader::Shader* shader, const char* entryName);
		void bindVertexLayout(VertexBinding* bindings,int binding_count);
		void bindRasterizationState(VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace, float lineWidth);
        void bindAttachments(AttachmentBinding* color_attachments, int color_attachments_count, AttachmentBinding* depth_stencil_attachment);
        void bindDescriptorSetLayouts(DescriptorSetLayout* descriptor_set_layouts, int descriptor_set_layouts_count);
        PSO createGraphicsPSO();
	private:
        VkPipelineCreateFlags                            flags{};
        VkPipelineRasterizationStateCreateInfo           pRasterizationState = DefaultPipelineRasterizationState();
        VkPipelineDepthStencilStateCreateInfo            pDepthStencilState = DefaultPipelineDepthStencilState();
        VkPipelineColorBlendStateCreateInfo              pColorBlendState{};
        VkPipelineInputAssemblyStateCreateInfo           pInputAssemblyState = DefaultPipelineInputAssemblyState();
        VkPipelineTessellationStateCreateInfo            pTessellationState = DefaultPipelineTessellationState();
        VkPipelineViewportStateCreateInfo                pViewportState = DefaultPipelineViewportState();
        VkPipelineMultisampleStateCreateInfo             pMultisampleState = DefaultPipelineMultisampleState();
        VkPipelineDynamicStateCreateInfo                 pDynamicState = DefaultPipelineDynamicState();
        uint32_t                                         subpass{};
        VkPipeline                                       basePipelineHandle{};
        int32_t                                          basePipelineIndex{};
        
        //vertex layout
        std::vector<VkVertexInputBindingDescription>     vertexLayout_bindings;
        std::vector<VkVertexInputAttributeDescription>   vertexLayout_attributes;
        VkPipelineVertexInputStateCreateInfo             pVertexInputState{};

        //render pass
        std::vector<VkAttachmentDescription>             renderPass_attachments;
        std::vector<VkAttachmentReference>               renderPass_attachmentRefs;
        VkSubpassDescription                             renderPass_subpass{};
        VkSubpassDependency                              renderPass_dependency{};
        VkRenderPassCreateInfo                           renderPass_create_info{};

        //descriptor set layout
        std::vector<std::vector<VkDescriptorSetLayoutBinding>> setLayout_bindings;
        std::vector<VkDescriptorSetLayoutCreateInfo>           sertLayout_createInfos;
        
        //shader
        uint32_t                                         stageCount{};
        ShaderCreateInfo                                 shaderInfo[5];
        VkPipelineShaderStageCreateInfo                  pStages[5];
	};
}