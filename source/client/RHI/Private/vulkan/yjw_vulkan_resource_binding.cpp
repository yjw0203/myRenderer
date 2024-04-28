#include "Private/vulkan/yjw_vulkan_resource_binding.h"
#include "Private/vulkan/yjw_vulkan_pipeline.h"
#include "Private/vulkan/yjw_vulkan_type_conversation.h"
#include "vulkan/vulkan.h"
#include <array>
#include <stdexcept>
#include <cassert>

namespace rhi
{
    VulkanResourceBinding::VulkanResourceBinding(VulkanDevice* pDevice, VulkanResourceLayoutView& layoutView, VkDescriptorSetLayout* pDescriptorSetLayout, int descriptorSetLayoutCount, std::unordered_map<RHIName, VulkanInputVertexBindingVariable>& input_reflect)
        :VulkanDeviceObject(pDevice)
    {
        assert(descriptorSetLayoutCount == layoutView.GetMaxSetCount());
        //poor
        std::array<VkDescriptorType, 3> descriptorType{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ,VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER };

        std::vector<VkDescriptorPoolSize> poolSizes{};
        poolSizes.reserve(5);
        for (int index = 0; index < descriptorType.size(); index++)
        {
            VkDescriptorPoolSize poolSize{};
            poolSize.type = descriptorType[index];
            poolSize.descriptorCount = layoutView.GetDescriptorCount(descriptorType[index]);
            if (poolSize.descriptorCount > 0)
            {
                poolSizes.push_back(poolSize);
            }
        }

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = layoutView.GetMaxSetCount();

        if (vkCreateDescriptorPool(pDevice->GetNativeDevice(), &poolInfo, nullptr, &m_descriptor_pool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }

        m_descriptor_sets.resize(layoutView.GetMaxSetCount());
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_descriptor_pool;
        allocInfo.descriptorSetCount = descriptorSetLayoutCount;
        allocInfo.pSetLayouts = pDescriptorSetLayout;

        VkResult hr;
        if ((hr = vkAllocateDescriptorSets(pDevice->GetNativeDevice(), &allocInfo, m_descriptor_sets.data())) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (int shaderType = 0; shaderType < (int)RHIShaderType::count; shaderType++)
        {
            m_reflection_tables[shaderType] = layoutView.GetReflectTableByShaderType((RHIShaderType)shaderType);
        }
        m_input_reflection = input_reflect;
        m_binding_count = m_input_reflection.size();
    }

    VulkanResourceBinding::~VulkanResourceBinding()
    {
        vkFreeDescriptorSets(GetDevice()->GetNativeDevice(), m_descriptor_pool, m_descriptor_sets.size(), m_descriptor_sets.data());
        vkDestroyDescriptorPool(GetDevice()->GetNativeDevice(), m_descriptor_pool, nullptr);
    }

    void VulkanResourceBinding::SetTextureView(RHIShaderType shaderType, RHIName name, RHITextureView* view)
    {
        auto iter = m_reflection_tables[(int)shaderType].find(name);
        if (iter == m_reflection_tables[(int)shaderType].end())
        {
            return;
        }
        VulkanResourceBindingVariable& variable = iter->second;
        if (variable.resourceType != sampled_image && variable.resourceType != separate_images)
        {
            return;
        }
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = VKResourceCast(view)->GetVkImageView();
        imageInfo.sampler = GetDevice()->m_default_sampler;

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = m_descriptor_sets[variable.setId];
        write.dstBinding = variable.binding;
        write.dstArrayElement = 0;
        write.descriptorType = ConvertShaderResourceTypeToDescriptorType(variable.resourceType);
        write.descriptorCount = 1;
        write.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(GetDevice()->GetNativeDevice(), 1, &write, 0, nullptr);

        m_binding_textures[(int)shaderType][name] = VKResourceCast(view);
    }

    void VulkanResourceBinding::SetBufferView(RHIShaderType shaderType, RHIName name, RHIBufferView* view)
    {
        auto iter = m_reflection_tables[(int)shaderType].find(name);
        if (iter == m_reflection_tables[(int)shaderType].end())
        {
            return;
        }
        VulkanResourceBindingVariable& variable = iter->second;
        if (variable.resourceType != uniform_buffer)
        {
            return;
        }
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = VKResourceCast(view)->GetBuffer()->GetVkBuffer();
        bufferInfo.offset = VKResourceCast(view)->GetDesc().offset;
        bufferInfo.range = VKResourceCast(view)->GetDesc().width;

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = m_descriptor_sets[variable.setId];
        write.dstBinding = variable.binding;
        write.dstArrayElement = 0;
        write.descriptorType = ConvertShaderResourceTypeToDescriptorType(variable.resourceType);
        write.descriptorCount = 1;
        write.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(GetDevice()->GetNativeDevice(), 1, &write, 0, nullptr);
    }

    void VulkanResourceBinding::SetVertexBuffer(RHIName name, RHIBuffer* buffer)
    {
        if (m_input_reflection.find(name) != m_input_reflection.end())
        {
            VulkanInputVertexBindingVariable& binding = m_input_reflection[name];
            m_vertex_buffers[binding.binding] = VKResourceCast(buffer);
            m_vertex_vkBuffers[binding.binding] = VKResourceCast(buffer)->GetVkBuffer();
            m_vertex_bufferOffsets[binding.binding] = 0;
        }
    }

    void VulkanResourceBinding::SetIndexBuffer(RHIBuffer* buffer)
    {
        m_index_buffer = VKResourceCast(buffer);
    }

    int VulkanResourceBinding::GetVertexBufferCount()
    {
        return m_binding_count;
    }

    VulkanBuffer* VulkanResourceBinding::GetVertexBuffer(int index)
    {
        return m_vertex_buffers[index];
    }

    VkBuffer* VulkanResourceBinding::GetVertexVkBuffers()
    {
        return m_vertex_vkBuffers;
    }
    VkDeviceSize* VulkanResourceBinding::GetVertexVkBufferOffsets()
    {
        return m_vertex_bufferOffsets;
    }

    VulkanBuffer* VulkanResourceBinding::GetIndexBuffer()
    {
        return m_index_buffer;
    }

    VkDescriptorSet* VulkanResourceBinding::GetDescriptorSetData()
    {
        return m_descriptor_sets.data();
    }

    int VulkanResourceBinding::GetDescriptorSetCount()
    {
        return m_descriptor_sets.size();
    }

    void VulkanResourceBinding::TransitionStateToRender(VkCommandBuffer commandBuffer)
    {
        for (int shaderType = 0; shaderType < (int)RHIShaderType::count; shaderType++)
        {
            for (auto& iter : m_binding_textures[shaderType])
            {
                if (iter.second)
                {
                    iter.second->GetTexture()->TransitionState(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
                }
            }
        }
    }
}