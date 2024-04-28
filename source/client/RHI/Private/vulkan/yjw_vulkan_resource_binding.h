#pragma once
#include "Public/RHI/rhi/yjw_rhi_resource_binding.h"
#include "Private/vulkan/yjw_vulkan_device.h"
#include "Private/vulkan/yjw_vulkan_texture.h"
#include "Private/vulkan/yjw_vulkan_buffer.h"

namespace rhi
{
    class VulkanResourceBinding : public RHIResourceBinding, VulkanDeviceObject
    {
    public:
        VulkanResourceBinding(VulkanDevice* pDevice, class VulkanResourceLayoutView& layoutView, VkDescriptorSetLayout* pDescriptorSetLayout,int descriptorSetLayoutCount, std::unordered_map<RHIName, VulkanInputVertexBindingVariable>& input_reflect);
        ~VulkanResourceBinding();
        virtual void SetTextureView(RHIShaderType shaderType, RHIName name, RHITextureView* view) override;
        virtual void SetBufferView(RHIShaderType shaderType, RHIName name, RHIBufferView* view) override;
        virtual void SetVertexBuffer(RHIName name, RHIBuffer* buffer);
        virtual void SetIndexBuffer(RHIBuffer* buffer);

        int GetVertexBufferCount();
        VulkanBuffer* GetVertexBuffer(int index);
        VkBuffer* GetVertexVkBuffers();
        VkDeviceSize* GetVertexVkBufferOffsets();
        VulkanBuffer* GetIndexBuffer();
        VkDescriptorSet* GetDescriptorSetData();
        int GetDescriptorSetCount();
        void TransitionStateToRender(VkCommandBuffer commandBuffer);
    private:
        VkDescriptorPool m_descriptor_pool;
        std::vector<VkDescriptorSet> m_descriptor_sets;
        std::unordered_map<RHIName, VulkanResourceBindingVariable> m_reflection_tables[(int)RHIShaderType::count];

        std::unordered_map<RHIName, VulkanInputVertexBindingVariable> m_input_reflection;
        int m_binding_count = 0;
        VulkanBuffer* m_vertex_buffers[VULKAN_MAX_VERTEX_BINDING] = {};
        VkBuffer m_vertex_vkBuffers[VULKAN_MAX_VERTEX_BINDING] = {};
        VkDeviceSize m_vertex_bufferOffsets[VULKAN_MAX_VERTEX_BINDING] = {};
        VulkanBuffer* m_index_buffer = nullptr;
        
        std::unordered_map<RHIName, VulkanTextureView*> m_binding_textures[(int)RHIShaderType::count];
    };
}