#include "Engine/RHI/Private/vulkan/yjw_vulkan_state_cache.h"

namespace rhi
{
    VulkanStateCache::VulkanStateCache(VulkanDevice* device)
        :VulkanDeviceObject(device)
    {
    }

    void VulkanStateCache::SetRenderPipeline(VulkanRenderPipeline* pipeline)
    {
        m_current_render_pipeline = pipeline;
    }

    VulkanRenderPipeline* VulkanStateCache::GetRenderPipeline()
    {
        return m_current_render_pipeline;
    }

    void VulkanStateCache::SetComputePipeline(VulkanComputePipeline* pipeline)
    {
        m_current_compute_pipeline = pipeline;
    }

    VulkanComputePipeline* VulkanStateCache::GetComputePipeline()
    {
        return m_current_compute_pipeline;
    }

    void VulkanStateCache::SetResourceSet(int set_id, VulkanResourceSet* resource_set)
    {
        while (set_id >= m_descriptor_set_count)
        {
            m_descriptor_sets[m_descriptor_set_count] = GetDevice()->m_default_descriptor_set;
            m_descriptor_set_count += 1;
        }
        if (resource_set != m_resource_sets[set_id])
        {
            m_descriptor_sets[set_id] = resource_set->GetDescriptorSet();
            if (m_resource_sets[set_id])
            {
                m_resource_sets[set_id]->release();
                m_resource_sets[set_id] = nullptr;
            }
            m_resource_sets[set_id] = resource_set;
            m_resource_sets[set_id]->retain(nullptr);
        }
    }

    VkDescriptorSet* VulkanStateCache::GetDescriptorSetData()
    {
        return m_descriptor_sets;
    }

    int VulkanStateCache::GetDescriptorSetCount()
    {
        return m_descriptor_set_count;
    }

    void VulkanStateCache::ClearResourceSet()
    {
        for (int i = 0; i < m_descriptor_set_count; i++)
        {
            if (m_resource_sets[i])
            {
                m_resource_sets[i]->release();
                m_resource_sets[i] = nullptr;
                m_descriptor_sets[i] = GetDevice()->m_default_descriptor_set;
            }
        }
        m_descriptor_set_count = 0;
    }

    void VulkanStateCache::SetPrimitiveBinding(VulkanPrimitiveBinding* primitiveBinding, int sub_id)
    {
        m_current_primitive_binding = primitiveBinding;
        m_current_sub_primitive_binding_id = sub_id;
    }

    VulkanPrimitiveBinding* VulkanStateCache::GetPrimitiveBinding()
    {
        return m_current_primitive_binding;
    }

    int VulkanStateCache::GetPrimitiveBindingSubID()
    {
        return m_current_sub_primitive_binding_id;
    }

    void VulkanStateCache::SetPushConstants(void* data, int offset, int size)
    {
        if (offset + size <= 128)
        {
            memcpy(m_push_constants + offset, data, size);
            m_push_constants_size = offset + size;
            m_push_constants_dirty = true;
        }
    }

    void* VulkanStateCache::GetPushConstantsData()
    {
        return m_push_constants;
    }

    int VulkanStateCache::GetPushConstantsSize()
    {
        return m_push_constants_size;
    }

    bool VulkanStateCache::IsPushConstantsDirty()
    {
        return m_push_constants_dirty;
    }

    void VulkanStateCache::ClearPushConstantsDirty()
    {
        memset(m_push_constants, 0, sizeof(m_push_constants));
        m_push_constants_dirty = false;
    }
}