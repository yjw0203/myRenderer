#pragma once
#include "vulkan/vulkan.h"
#include "rhi/vulkan/yjw_vulkan_default_states.h"
#include <vector>
#include "shader/yjw_shader_define.h"
#include "rhi/vulkan/yjw_vulkan_resource_ruler.h"

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
        VulkanShader* shader;
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
        std::vector<AttachmentBinding> color_attachments;
        bool has_depth_stencil_attachment{};
        AttachmentBinding depth_stencil_attachment;
    };

    struct PSODescriptorLayoutBinding
    {
        std::vector<std::vector<DescriptorSetLayoutBinding>> bindings;
        int queryCount(VkDescriptorType type);
    };
    
    class ShaderCreateInfo
    {
    public:
        VkShaderStageFlagBits stage;
        VkShaderModule module;
        std::string entryName;
    };

    class VulkanPSOCreation;
    class VulkanPSO
    {
    public:
        static const VulkanResourceType TypeId = VulkanResourceType::pso;
        typedef VulkanPSOCreation Creation;
        VkRenderPass renderPass{};
        VkPipelineLayout pipelineLayout{};
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        VkPipeline pso{};
        PSODescriptorLayoutBinding descriptorLayoutBinding;
        int attachmentCount;
    };

	class VulkanPSOCreation
	{
	public:
        void bind(PSOShaderBinding* bind);
		void bind(PSOVertexBinding* bind);
		void bind(PSORasterizationStateBinding* bind);
        void bind(PSOAttachmentBinding* bind);
        void bind(PSODescriptorLayoutBinding* bind);
	public:
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
        
        VkPipelineVertexInputStateCreateInfo             pVertexInputState{};
        VkRenderPassCreateInfo                           renderPass_create_info{};
        std::vector<VkDescriptorSetLayoutCreateInfo>     sertLayout_createInfos;
        
        //shader
        ShaderCreateInfo                                 shaderInfo[5];
        uint32_t                                         stageCount{};
        VkPipelineShaderStageCreateInfo                  pStages[5];

        PSODescriptorLayoutBinding                       descriptorLayoutBinding;
    private:
        //vertex layout
        std::vector<VkVertexInputBindingDescription>     vertexLayout_bindings;
        std::vector<VkVertexInputAttributeDescription>   vertexLayout_attributes;

        //render pass
        std::vector<VkAttachmentDescription>             renderPass_attachments;
        std::vector<VkAttachmentReference>               renderPass_attachmentRefs;
        VkSubpassDescription                             renderPass_subpass{};
        VkSubpassDependency                              renderPass_dependency{};

        //descriptor set layout
        std::vector<std::vector<VkDescriptorSetLayoutBinding>> setLayout_bindings;
        
	};

    typedef ResourceHandle<VulkanPSO> VulkanPSOHandle;

    struct DefaultVulkanPSOAllocateStrategy
    {
        VulkanPSO* CreateFunc(VulkanPSOCreation& creation);
        void DestoryFunc(VulkanPSO* resource);
    };

    class VulkanPSOPool
    {
    public:
        VulkanPSOHandle createPSO(VulkanPSOCreation& initConfig);
        void destroyPSO(VulkanPSOHandle handle);

    private:
        ResourceAllocator<VulkanPSO, DefaultVulkanPSOAllocateStrategy> DefaultAllocator;
    };

    EXTERN_GLOBAL_REF(VulkanPSOPool);
}
