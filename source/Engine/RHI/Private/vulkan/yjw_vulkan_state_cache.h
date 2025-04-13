#pragma once
#include "Engine/RHI/Private/vulkan/yjw_vulkan_pipeline.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_resource_binding.h"

namespace rhi
{
    class VulkanStateCache : public RHIObject, VulkanDeviceObject
    {
    public:
        VulkanStateCache(VulkanDevice* device);
        void SetRenderPipeline(VulkanRenderPipeline* pipeline);
        VulkanRenderPipeline* GetRenderPipeline();
        void SetComputePipeline(VulkanComputePipeline* pipeline);
        VulkanComputePipeline* GetComputePipeline();
        void SetResourceSet(int set_id, VulkanResourceSet* resource_set);
        VkDescriptorSet* GetDescriptorSetData();
        int GetDescriptorSetCount();
        void ClearResourceSet();
        void SetPrimitiveBinding(VulkanPrimitiveBinding* primitiveBinding,int sub_id);
        VulkanPrimitiveBinding* GetPrimitiveBinding();
        int GetPrimitiveBindingSubID();

        void SetPushConstants(void* data, int offset, int size);
        void* GetPushConstantsData();
        int GetPushConstantsSize();
        bool IsPushConstantsDirty();
        void ClearPushConstantsDirty();
    private:
        VulkanRenderPipeline* m_current_render_pipeline = nullptr;
        VulkanComputePipeline* m_current_compute_pipeline = nullptr;
        VulkanPrimitiveBinding* m_current_primitive_binding = nullptr;

        VulkanResourceSet* m_resource_sets[16] = {};
        VkDescriptorSet m_descriptor_sets[16] = {};
        int m_descriptor_set_count = 0;

        int m_current_sub_primitive_binding_id{};
        char m_push_constants[128] = {};
        bool m_push_constants_dirty = false;
        int m_push_constants_size = 0;
    };
}