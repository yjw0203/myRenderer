#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_resource_binding.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_texture.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_buffer.h"

namespace rhi
{
    class VulkanResourceBinding : public RHIResourceBinding, VulkanDeviceObject
    {
    public:
        VulkanResourceBinding(VulkanDevice* pDevice, class VulkanResourceLayoutView& reflectView, VkDescriptorSetLayout* pDescriptorSetLayout,int descriptorSetLayoutCount);
        ~VulkanResourceBinding();
        virtual void SetTextureView(RHIShaderType shaderType, RHIName name, RHITextureView* view) override;
        virtual void SetBufferView(RHIShaderType shaderType, RHIName name, RHIBufferView* view) override;

        VkDescriptorSet* GetDescriptorSetData();
        int GetDescriptorSetCount();
        void TransitionStateToRender(VkCommandBuffer commandBuffer);
    private:
        VulkanResourceLayoutView& m_reflect_view;

        VkDescriptorPool m_descriptor_pool;
        std::vector<VkDescriptorSet> m_descriptor_sets;
        std::unordered_map<RHIName, VulkanTextureView*> m_binding_textures[(int)RHIShaderType::count];
    };

    class VulkanPrimitiveBinding : public RHIPrimitiveBinding, VulkanDeviceObject
    {
    public:
        VulkanPrimitiveBinding(VulkanDevice* pDevice, class ShaderReflect& reflect);
        ~VulkanPrimitiveBinding();
        virtual void SetVertexBuffer(RHIName name, RHIBufferView* bufferView) override;
        virtual void SetIndexBuffer(RHIBufferView* buffer, int intdex_start, int index_count, bool is_index_16bit) override;

        int GetVertexBufferCount();
        VulkanBuffer* GetVertexBuffer(int index);
        VkBuffer* GetVertexVkBuffers();
        VkDeviceSize* GetVertexVkBufferOffsets();
        VulkanBuffer* GetIndexBuffer();
        int GetIndexBufferOffset();
        int GetIndexStart();
        int GetIndexCount();
        bool GetIsIndex16Bit();
        int GetVertexOffset();
    private:
        ShaderReflect& m_reflect;

        VulkanBuffer* m_vertex_buffers[VULKAN_MAX_VERTEX_BINDING] = {};
        VkBuffer m_vertex_vkBuffers[VULKAN_MAX_VERTEX_BINDING] = {};
        VkDeviceSize m_vertex_bufferOffsets[VULKAN_MAX_VERTEX_BINDING] = {};
        VulkanBuffer* m_index_buffer = nullptr;
        int m_index_buffer_offset = 0;
        int m_index_start = 0;
        int m_index_count = 0;
        bool m_is_index_16bit = false;
        int m_vertex_offset = 0;
    };
}