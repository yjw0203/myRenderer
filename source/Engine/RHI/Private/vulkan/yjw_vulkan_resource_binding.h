#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_resource_binding.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_texture.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_buffer.h"

namespace rhi
{
    class VulkanResourceSet : public RHIResourceSet, VulkanDeviceObject
    {
    public:
        struct VariableBinding
        {
            VkDescriptorType type;
            int set;
            int binding;
        };
        VulkanResourceSet(VulkanDevice* pDevice, int set_id, const ShaderReflect& reflection);
        ~VulkanResourceSet();
        virtual void SetTextureView(RHIName name, RHITextureView* view) override;
        virtual void SetBufferView(RHIName name, RHIBufferView* view) override;
        VkDescriptorSet GetDescriptorSet();

        void TransitionStateToRender(VkCommandBuffer commandBuffer);
    private:
        const VariableBinding* GetVariableBinding(RHIName name);
    private:
        VkDescriptorSetLayout m_descriptor_set_layout;
        VkDescriptorPool m_descriptor_pool;
        VkDescriptorSet m_descriptor_set;
        std::unordered_map<RHIName, VariableBinding> m_resource_table;
        std::unordered_map<RHIName, VulkanTextureView*> m_binding_textures;
    };

    class VulkanPrimitiveBinding : public RHIPrimitiveBinding, VulkanDeviceObject
    {
    public:
        VulkanPrimitiveBinding(VulkanDevice* pDevice, VulkanShader* vs);
        ~VulkanPrimitiveBinding();
        virtual RHIShader* GetVertexShader() override;
        virtual void SetVertexBuffer(RHIName name, RHIBufferView* bufferView) override;
        virtual int AddIndexBuffer(RHIBufferView* buffer, int intdex_start, int index_count, bool is_index_16bit) override;

        int GetVertexBufferCount();
        VulkanBuffer* GetVertexBuffer(int index);
        VkBuffer* GetVertexVkBuffers();
        VkDeviceSize* GetVertexVkBufferOffsets();
        VulkanBuffer* GetIndexBuffer(int id);
        int GetIndexBufferOffset(int id);
        int GetIndexStart(int id);
        int GetIndexCount(int id);
        bool GetIsIndex16Bit(int id);
        int GetVertexOffset();
    private:
        VulkanShader* m_vs{};

        VulkanBuffer* m_vertex_buffers[VULKAN_MAX_VERTEX_BINDING] = {};
        VkBuffer m_vertex_vkBuffers[VULKAN_MAX_VERTEX_BINDING] = {};
        VkDeviceSize m_vertex_bufferOffsets[VULKAN_MAX_VERTEX_BINDING] = {};
        int m_vertex_offset = 0;

        struct SubPrimitiveBinding
        {
            VulkanBuffer* m_index_buffer = nullptr;
            int m_index_buffer_offset = 0;
            int m_index_start = 0;
            int m_index_count = 0;
            bool m_is_index_16bit = false;
        };
        std::vector<SubPrimitiveBinding> m_sub_primitive_binding;
    };
}