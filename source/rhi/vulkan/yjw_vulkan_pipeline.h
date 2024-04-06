#pragma once
#include "RHI/rhi/yjw_rhi_pipeline.h"
#include "RHI/rhi/yjw_rhi_common.h"
#include "yjw_vulkan_define.h"
#include "yjw_vulkan_device.h"
#include <unordered_map>
#include "vulkan/vulkan.h"
#include "yjw_vulkan_render_pass.h"

namespace rhi
{
    class VulkanDevice;

    class VulkanResourceLayoutView
    {
    public:
        VulkanResourceLayoutView() {};
        void AddReflectionTable(RHIShaderType shaderType, std::unordered_map<RHIName, VulkanResourceBindingVariable>& reflection_table);
        std::vector<VkDescriptorSetLayoutBinding>& GetBindingsBySetID(int setId);
        std::unordered_map<RHIName, VulkanResourceBindingVariable>& GetReflectTableByShaderType(RHIShaderType shaderType);
        int GetMaxSetCount();
        int GetDescriptorCount(VkDescriptorType type);
    private:
        std::vector<VkDescriptorSetLayoutBinding> m_sets[VULKAN_MAX_DESCRIPTOR_SET];
        std::unordered_map<RHIName, VulkanResourceBindingVariable> m_reflect_table[(int)RHIShaderType::count];
        int m_max_set_id = -1;
    };

    class VulkanRenderPipeline : public RHIRenderPipeline, VulkanDeviceObject
    {
    public:
        VulkanRenderPipeline(VulkanDevice* device,const RHIRenderPipelineDescriptor& desc);
        ~VulkanRenderPipeline();
        virtual RHIResourceBinding* CreateResourceBinding() override;

        VkPipeline GetOrCreateVkPipeline(VulkanRenderPass* renderPass);
    private:
        VkPipelineLayout GetOrCreateVkPipelineLayout();
    private:
        VkPipelineLayout m_pipeline_layout = nullptr;
        std::vector<VkDescriptorSetLayout> m_descriptor_set_layouts;
        std::unordered_map<VulkanRenderPass*, VkPipeline> m_pipelines;
    };
}