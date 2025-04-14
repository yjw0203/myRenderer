#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_pipeline.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_common.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_define.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_render_pass.h"
#include <unordered_map>
#include "vulkan/vulkan.h"
#include "Engine/RHI/Public/shaderCompiler/yjw_shader_compiler.h"

namespace rhi
{
    class VulkanDevice;

    class VulkanResourceLayoutView
    {
    public:
        struct VariableBinding
        {
            VkDescriptorType type;
            int set;
            int binding;
        };
        VulkanResourceLayoutView() {};
        void AddReflectionTable(RHIShaderType shaderType, const ShaderReflect& reflection);
        void OverrideSetReflection(int set, VkShaderStageFlags shaderBits, const ShaderReflect& reflection);
        void AddBinding(VkShaderStageFlags shaderBits,RHIName name, int set, int binding, VkDescriptorType descriptorType);
        std::vector<VkDescriptorSetLayoutBinding>& GetBindingsBySetID(int setId);
        int GetMaxSetCount();
        int GetVertexBindingCount();
        int GetVertexInputLocation(const RHIName& name);
    private:
        std::vector<VkDescriptorSetLayoutBinding> m_sets[VULKAN_MAX_DESCRIPTOR_SET];
        std::unordered_map<RHIName, int> m_vertex_input_table;
        int m_max_set_id = -1;
    };

    class VulkanRenderPipeline : public RHIRenderPipeline, VulkanDeviceObject
    {
    public:
        VulkanRenderPipeline(VulkanDevice* device,const RHIRenderPipelineDescriptor& desc);
        ~VulkanRenderPipeline();
        virtual RHIPrimitiveBinding* CreatePrimitiveBinding() override;
        virtual ShaderReflect* GetVSShaderReflect() override;
        virtual ShaderReflect* GetPSShaderReflect() override;
        VkPipeline GetOrCreateVkPipeline(VulkanRenderPass* renderPass);
        VkPipelineLayout GetOrCreateVkPipelineLayout();
    private:
        VkPipelineLayout m_pipeline_layout = nullptr;
        std::vector<VkDescriptorSetLayout> m_descriptor_set_layouts;
        std::unordered_map<VulkanRenderPass*, VkPipeline> m_pipelines;
        VulkanResourceLayoutView m_reflect_view{};
    };

    class VulkanComputePipeline : public RHIComputePipeline, VulkanDeviceObject
    {
    public:
        VulkanComputePipeline(VulkanDevice* device, const RHIComputePipelineDescriptor& desc);
        ~VulkanComputePipeline();
        VkPipeline GetOrCreateVkPipeline();
        VkPipelineLayout GetOrCreateVkPipelineLayout();
    private:
        VkPipelineLayout m_pipeline_layout = nullptr;
        std::vector<VkDescriptorSetLayout> m_descriptor_set_layouts;
        VkPipeline m_pipeline = nullptr;
        VulkanResourceLayoutView m_reflect_view{};
    };
}