#include "Private/vulkan/yjw_vulkan_resource_binding.h"
#include "Private/vulkan/yjw_vulkan_pipeline.h"
#include "Private/vulkan/yjw_vulkan_type_conversation.h"
#include "vulkan/vulkan.h"
#include <array>
#include <stdexcept>
#include <cassert>

namespace rhi
{
    VulkanResourceBinding::VulkanResourceBinding(VulkanDevice* pDevice, VulkanResourceLayoutView& reflectView, VkDescriptorSetLayout* pDescriptorSetLayout, int descriptorSetLayoutCount)
        :VulkanDeviceObject(pDevice),m_reflect_view(reflectView)
    {
        assert(descriptorSetLayoutCount == reflectView.GetMaxSetCount());
        //poor
        std::array<VkDescriptorType, 3> descriptorType{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ,VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER };

        std::vector<VkDescriptorPoolSize> poolSizes{};
        poolSizes.reserve(5);
        for (int index = 0; index < descriptorType.size(); index++)
        {
            VkDescriptorPoolSize poolSize{};
            poolSize.type = descriptorType[index];
            poolSize.descriptorCount = reflectView.GetDescriptorCount(descriptorType[index]);
            if (poolSize.descriptorCount > 0)
            {
                poolSizes.push_back(poolSize);
            }
        }

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = reflectView.GetMaxSetCount();

        if (vkCreateDescriptorPool(pDevice->GetNativeDevice(), &poolInfo, nullptr, &m_descriptor_pool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }

        m_descriptor_sets.resize(reflectView.GetMaxSetCount());
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_descriptor_pool;
        allocInfo.descriptorSetCount = descriptorSetLayoutCount;
        allocInfo.pSetLayouts = pDescriptorSetLayout;

        VkResult hr;
        if ((hr = vkAllocateDescriptorSets(pDevice->GetNativeDevice(), &allocInfo, m_descriptor_sets.data())) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
    }

    VulkanResourceBinding::~VulkanResourceBinding()
    {
        vkFreeDescriptorSets(GetDevice()->GetNativeDevice(), m_descriptor_pool, m_descriptor_sets.size(), m_descriptor_sets.data());
        vkDestroyDescriptorPool(GetDevice()->GetNativeDevice(), m_descriptor_pool, nullptr);
    }

    void VulkanResourceBinding::SetTextureView(RHIShaderType shaderType, RHIName name, RHITextureView* view)
    {
        const VulkanResourceLayoutView::VariableBinding* variable = m_reflect_view.GetVariableBinding(shaderType, name);
        if (!variable)
        {
            return;
        }
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = VKResourceCast(view)->GetVkImageView();
        imageInfo.sampler = GetDevice()->m_default_sampler;

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = m_descriptor_sets[variable->set];
        write.dstBinding = variable->binding;
        write.dstArrayElement = 0;
        write.descriptorType = variable->type;
        write.descriptorCount = 1;
        write.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(GetDevice()->GetNativeDevice(), 1, &write, 0, nullptr);

        m_binding_textures[(int)shaderType][name] = VKResourceCast(view);
    }

    void VulkanResourceBinding::SetBufferView(RHIShaderType shaderType, RHIName name, RHIBufferView* view)
    {
        const VulkanResourceLayoutView::VariableBinding* variable = m_reflect_view.GetVariableBinding(shaderType, name);
        if (!variable)
        {
            return;
        }
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = VKResourceCast(view)->GetBuffer()->GetVkBuffer();
        bufferInfo.offset = VKResourceCast(view)->GetDesc().offset;
        bufferInfo.range = VKResourceCast(view)->GetDesc().width;

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = m_descriptor_sets[variable->set];
        write.dstBinding = variable->binding;
        write.dstArrayElement = 0;
        write.descriptorType = variable->type;
        write.descriptorCount = 1;
        write.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(GetDevice()->GetNativeDevice(), 1, &write, 0, nullptr);
    }

    void VulkanResourceBinding::SetVertexBuffer(RHIName name, RHIBuffer* buffer)
    {
        int location = m_reflect_view.GetVertexInputLocation(name);
        if (location >= 0)
        {
            m_vertex_buffers[location] = VKResourceCast(buffer);
            m_vertex_vkBuffers[location] = VKResourceCast(buffer)->GetVkBuffer();
            m_vertex_bufferOffsets[location] = 0;
        }
    }

    void VulkanResourceBinding::SetIndexBuffer(RHIBuffer* buffer)
    {
        m_index_buffer = VKResourceCast(buffer);
    }

    int VulkanResourceBinding::GetVertexBufferCount()
    {
        return m_reflect_view.GetVertexBindingCount();
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