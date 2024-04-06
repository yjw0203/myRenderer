#include "yjw_vulkan_resource_binding.h"
#include "yjw_vulkan_pipeline.h"
#include "yjw_vulkan_type_conversation.h"
#include "vulkan/vulkan.h"
#include <array>
#include <stdexcept>
#include <cassert>

namespace rhi
{
    VulkanResourceBinding::VulkanResourceBinding(VulkanDevice* pDevice, VulkanResourceLayoutView& layoutView, VkDescriptorSetLayout* pDescriptorSetLayout, int descriptorSetLayoutCount)
        :VulkanDeviceObject(pDevice)
    {
        assert(descriptorSetLayoutCount == layoutView.GetMaxSetCount());
        //poor
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[0].descriptorCount = layoutView.GetDescriptorCount(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[1].descriptorCount = layoutView.GetDescriptorCount(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

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
        if (variable.resourceType != sampled_image)
        {
            return;
        }
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = ResourceCast(view)->GetVkImageView();
        imageInfo.sampler = nullptr/*VK_G(VulkanDefaultResource).DefaultSampler*/;

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = m_descriptor_sets[variable.setId];
        write.dstBinding = variable.binding;
        write.dstArrayElement = 0;
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.descriptorCount = 1;
        write.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(GetDevice()->GetNativeDevice(), 1, &write, 0, nullptr);
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
        bufferInfo.buffer = ResourceCast(view)->GetBuffer()->GetVkBuffer();
        bufferInfo.offset = ResourceCast(view)->GetDesc().offset;
        bufferInfo.range = ResourceCast(view)->GetDesc().width;

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = m_descriptor_sets[variable.setId];
        write.dstBinding = variable.binding;
        write.dstArrayElement = 0;
        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write.descriptorCount = 1;
        write.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(GetDevice()->GetNativeDevice(), 1, &write, 0, nullptr);
    }
}