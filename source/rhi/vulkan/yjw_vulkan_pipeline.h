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
        void AddReflectionTable(VulkanShaderType shaderType, std::unordered_map<RHIName, VulkanResourceBinding>& reflection_table);
        std::vector<VkDescriptorSetLayoutBinding>& GetBindingsBySetID(int setId);
        int GetMaxSetCount();
    private:
        std::vector<VkDescriptorSetLayoutBinding> m_sets[VULKAN_MAX_DESCRIPTOR_SET];
        int m_max_set_count = -1;
    };

    class VulkanRenderPipeline : public RHIRenderPipeline, VulkanDeviceObject
    {
    public:
        VulkanRenderPipeline(VulkanDevice* device,const RHIRenderPipelineDescriptor& desc);
        ~VulkanRenderPipeline();
        VkPipeline GetOrCreateVkPipeline(VulkanRenderPass* renderPass);
    private:
        VkPipelineLayout GetOrCreateVkPipelineLayout();
        VkPipelineLayout m_pipeline_layout = nullptr;
        std::unordered_map<VulkanRenderPass*, VkPipeline> m_pipelines;
    };
}