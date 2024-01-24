#pragma once
#include "vulkan/vulkan.h"
#include "rhi/vulkan/yjw_vulkan_default_states.h"
#include <vector>
#include "shader/yjw_shader_define.h"

namespace vulkan
{
    class VertexLocation
    {
    public:
        int location{};
        VkFormat format{};
    };

    class AttachmentBinding
    {
    public:
        VkFormat format{};
    };

    class DescriptorSetLayoutBinding
    {
    public:
        DescriptorSetLayoutBinding(int in_slot, VkDescriptorType in_type, VkShaderStageFlags in_shaderStage) :slot(in_slot), type(in_type), shaderStage(in_shaderStage)
        {}
        VkShaderStageFlags shaderStage{};
        int slot{};
        VkDescriptorType type{};
    };

    // pso bindings
    struct PSOShaderBinding
    {
        Shader* shader;
        std::string entryName;
    };

    struct PSOVertexBinding
    {
        std::vector<std::vector<VertexLocation>> locations;
    };

    struct PSORasterizationStateBinding
    {
        VkPolygonMode polygonMode;
        VkCullModeFlags cullMode;
        VkFrontFace frontFace;
        float lineWidth;
    };

    struct PSOAttachmentBinding
    {
        AttachmentBinding* color_attachments;
        int color_attachments_count;
        AttachmentBinding* depth_stencil_attachment;
    };

    struct PSODescriptorLayoutBinding
    {
        std::vector<std::vector<DescriptorSetLayoutBinding>> bindings;
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
        void bind(PSOShaderBinding* bind);
		void bind(PSOVertexBinding* bind);
		void bind(PSORasterizationStateBinding* bind);
        void bind(PSOAttachmentBinding* bind);
        void bind(PSODescriptorLayoutBinding* bind);
        PSO* createGraphicsPSO();
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
