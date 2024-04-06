#pragma once
#include "RHI/rhi/yjw_rhi_resource_binding.h"
#include "yjw_vulkan_device.h"
#include "yjw_vulkan_texture.h"
#include "yjw_vulkan_buffer.h"

namespace rhi
{
    class VulkanResourceBinding : public RHIResourceBinding, VulkanDeviceObject
    {
    public:
        VulkanResourceBinding(VulkanDevice* pDevice, class VulkanResourceLayoutView& layoutView, VkDescriptorSetLayout* pDescriptorSetLayout,int descriptorSetLayoutCount);
        ~VulkanResourceBinding();
        virtual void SetTextureView(RHIShaderType shaderType, RHIName name, RHITextureView* view) override;
        virtual void SetBufferView(RHIShaderType shaderType, RHIName name, RHIBufferView* view) override;

    private:
        VkDescriptorPool m_descriptor_pool;
        std::vector<VkDescriptorSet> m_descriptor_sets;
        std::unordered_map<RHIName, VulkanResourceBindingVariable> m_reflection_tables[(int)RHIShaderType::count];
    };
}