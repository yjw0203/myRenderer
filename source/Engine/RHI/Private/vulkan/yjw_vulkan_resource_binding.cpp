#include "Engine/RHI/Private/vulkan/yjw_vulkan_resource_binding.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_pipeline.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_type_conversation.h"
#include "vulkan/vulkan.h"
#include <array>
#include <stdexcept>
#include <cassert>

namespace rhi
{
    VulkanResourceSet::VulkanResourceSet(VulkanDevice* pDevice, int set_id, const ShaderReflect& reflection)
        :VulkanDeviceObject(pDevice)
    {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        bindings.reserve(5);

        int descriptor_uniform_buffer_count = 0;
        for (const ShaderReflect::UBO& ubo : reflection.m_ubos)
        {
            if (ubo.m_set == set_id)
            {
                m_resource_table[RHIName(ubo.m_name)] = VariableBinding{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER , ubo.m_set, ubo.m_binding };
                VkDescriptorSetLayoutBinding binding{};
                binding.binding = ubo.m_binding;
                binding.descriptorCount = 1;
                binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                binding.pImmutableSamplers = nullptr;
                binding.stageFlags = VK_SHADER_STAGE_ALL;
                bindings.push_back(binding);
                descriptor_uniform_buffer_count++;
            }
        }
        int descriptor_combined_image_count = 0;
        for (const ShaderReflect::SeparateImage& image : reflection.m_separate_images)
        {
            if (image.m_set == set_id)
            {
                m_resource_table[RHIName(image.m_name)] = VariableBinding{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER , image.m_set, image.m_binding };
                VkDescriptorSetLayoutBinding binding{};
                binding.binding = image.m_binding;
                binding.descriptorCount = 1;
                binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                binding.pImmutableSamplers = nullptr;
                binding.stageFlags = VK_SHADER_STAGE_ALL;
                bindings.push_back(binding);
                descriptor_combined_image_count++;
            }
        }
        int descriptor_uniform_texel_buffer_count = 0;
        for (const ShaderReflect::SamplerBuffer& buffer : reflection.m_sampler_buffers)
        {
            if (buffer.m_set == set_id)
            {
                m_resource_table[RHIName(buffer.m_name)] = VariableBinding{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER , buffer.m_set, buffer.m_binding };
                VkDescriptorSetLayoutBinding binding{};
                binding.binding = buffer.m_binding;
                binding.descriptorCount = 1;
                binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
                binding.pImmutableSamplers = nullptr;
                binding.stageFlags = VK_SHADER_STAGE_ALL;
                bindings.push_back(binding);
                descriptor_uniform_texel_buffer_count++;
            }
        }

        VkDescriptorSetLayoutCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.bindingCount = bindings.size();
        createInfo.pBindings = bindings.data();
        createInfo.flags = 0;
        createInfo.pNext = nullptr;
        vkCreateDescriptorSetLayout(GetDevice()->GetNativeDevice(), &createInfo, nullptr, &m_descriptor_set_layout);

        std::array<VkDescriptorType, 3> descriptorType{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ,VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER };
        std::array<int, 3> descriptorCount{ descriptor_combined_image_count, descriptor_uniform_buffer_count ,descriptor_uniform_texel_buffer_count };

        std::vector<VkDescriptorPoolSize> poolSizes{};
        poolSizes.reserve(5);
        for (int index = 0; index < descriptorType.size(); index++)
        {
            VkDescriptorPoolSize poolSize{};
            poolSize.type = descriptorType[index];
            poolSize.descriptorCount = descriptorCount[index];
            if (poolSize.descriptorCount > 0)
            {
                poolSizes.push_back(poolSize);
            }
        }

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = poolSizes.size();
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = 1;

        if (vkCreateDescriptorPool(pDevice->GetNativeDevice(), &poolInfo, nullptr, &m_descriptor_pool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }


        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_descriptor_pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &m_descriptor_set_layout;

        VkResult hr;
        if ((hr = vkAllocateDescriptorSets(pDevice->GetNativeDevice(), &allocInfo, &m_descriptor_set)) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
    }

    VulkanResourceSet::~VulkanResourceSet()
    {
        vkFreeDescriptorSets(GetDevice()->GetNativeDevice(), m_descriptor_pool, 1, &m_descriptor_set);
        vkDestroyDescriptorPool(GetDevice()->GetNativeDevice(), m_descriptor_pool, nullptr);
        vkDestroyDescriptorSetLayout(GetDevice()->GetNativeDevice(), m_descriptor_set_layout, nullptr);
    }

    void VulkanResourceSet::SetTextureView(RHIName name, RHITextureView* view)
    {
        const VulkanResourceSet::VariableBinding* variable = GetVariableBinding(name);
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
        write.dstSet = m_descriptor_set;
        write.dstBinding = variable->binding;
        write.dstArrayElement = 0;
        write.descriptorType = variable->type;
        write.descriptorCount = 1;
        write.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(GetDevice()->GetNativeDevice(), 1, &write, 0, nullptr);

        m_binding_textures[name] = VKResourceCast(view);
    }

    void VulkanResourceSet::SetBufferView(RHIName name, RHIBufferView* view)
    {
        const VulkanResourceSet::VariableBinding* variable = GetVariableBinding(name);
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
        write.dstSet = m_descriptor_set;
        write.dstBinding = variable->binding;
        write.dstArrayElement = 0;
        write.descriptorType = variable->type;
        write.descriptorCount = 1;
        write.pBufferInfo = &bufferInfo;
        if (VKResourceCast(view)->GetView())
        {
            write.pTexelBufferView = &(VKResourceCast(view)->GetView());
        }

        vkUpdateDescriptorSets(GetDevice()->GetNativeDevice(), 1, &write, 0, nullptr);
    }

    VkDescriptorSet VulkanResourceSet::GetDescriptorSet()
    {
        return m_descriptor_set;
    }

    void VulkanResourceSet::TransitionStateToRender(VkCommandBuffer commandBuffer)
    {
        for (auto& iter : m_binding_textures)
        {
            if (iter.second)
            {
                iter.second->GetTexture()->TransitionState(commandBuffer, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            }
        }
    }

    const VulkanResourceSet::VariableBinding* VulkanResourceSet::GetVariableBinding(RHIName name)
    {
        if (m_resource_table.find(name) == m_resource_table.end())
        {
            return nullptr;
        }
        return &m_resource_table[name];
    }

    VulkanResourceBinding::VulkanResourceBinding(VulkanDevice* pDevice)
        :VulkanDeviceObject(pDevice)
    {
    }

    VulkanResourceBinding::~VulkanResourceBinding()
    {
    }

    void VulkanResourceBinding::SetResourceSet(int set_id, RHIResourceSet* set)
    {
        if (m_descriptor_sets.size() <= set_id)
        {
            m_descriptor_sets.resize(set_id + 1, GetDevice()->m_default_descriptor_set);
            m_resource_set_handles.resize(set_id + 1);
        }
        if (m_resource_set_handles[set_id] != nullptr)
        {
            m_resource_set_handles[set_id]->release();
            m_resource_set_handles[set_id] = nullptr;
            m_descriptor_sets[set_id] = nullptr;
        }
        m_descriptor_sets[set_id] = ((VulkanResourceSet*)set)->GetDescriptorSet();
        m_resource_set_handles[set_id] = (VulkanResourceSet*)set;
        set->retain(this);
    }

    VkDescriptorSet* VulkanResourceBinding::GetDescriptorSetData()
    {
        return m_descriptor_sets.data();
    }

    int VulkanResourceBinding::GetDescriptorSetCount()
    {
        return m_descriptor_sets.size();
    }

    VulkanPrimitiveBinding::VulkanPrimitiveBinding(VulkanDevice* pDevice, ShaderReflect& reflect)
        :VulkanDeviceObject(pDevice), m_reflect(reflect)
    {
    }

    VulkanPrimitiveBinding::~VulkanPrimitiveBinding()
    {
    }

    void VulkanPrimitiveBinding::SetVertexBuffer(RHIName name, RHIBufferView* bufferView)
    {
        int location = m_reflect.GetVertexInputLocation(name);
        if (location >= 0)
        {
            m_vertex_buffers[location] = VKResourceCast(bufferView)->GetBuffer();
            m_vertex_vkBuffers[location] = VKResourceCast(bufferView)->GetBuffer()->GetVkBuffer();
            m_vertex_bufferOffsets[location] = VKResourceCast(bufferView)->GetOffset();
        }
    }

    int VulkanPrimitiveBinding::AddIndexBuffer(RHIBufferView* bufferView, int index_start, int index_count, bool is_index_16bit)
    {
        int id = m_sub_primitive_binding.size();
        m_sub_primitive_binding.push_back(SubPrimitiveBinding{});
        m_sub_primitive_binding.back().m_index_buffer = VKResourceCast(bufferView)->GetBuffer();
        m_sub_primitive_binding.back().m_index_buffer_offset = VKResourceCast(bufferView)->GetOffset();
        m_sub_primitive_binding.back().m_index_start = index_start;
        m_sub_primitive_binding.back().m_index_count = index_count;
        m_sub_primitive_binding.back().m_is_index_16bit = is_index_16bit;
        return id;
    }

    int VulkanPrimitiveBinding::GetVertexBufferCount()
    {
        return m_reflect.GetVertexBindingCount();
    }

    VulkanBuffer* VulkanPrimitiveBinding::GetVertexBuffer(int index)
    {
        return m_vertex_buffers[index];
    }

    VkBuffer* VulkanPrimitiveBinding::GetVertexVkBuffers()
    {
        return m_vertex_vkBuffers;
    }
    VkDeviceSize* VulkanPrimitiveBinding::GetVertexVkBufferOffsets()
    {
        return m_vertex_bufferOffsets;
    }

    VulkanBuffer* VulkanPrimitiveBinding::GetIndexBuffer(int id)
    {
        return m_sub_primitive_binding[id].m_index_buffer;
    }

    int VulkanPrimitiveBinding::GetIndexBufferOffset(int id)
    {
        return m_sub_primitive_binding[id].m_index_buffer_offset;
    }

    int VulkanPrimitiveBinding::GetIndexStart(int id)
    {
        return m_sub_primitive_binding[id].m_index_start;
    }

    int VulkanPrimitiveBinding::GetIndexCount(int id)
    {
        return m_sub_primitive_binding[id].m_index_count;
    }

    bool VulkanPrimitiveBinding::GetIsIndex16Bit(int id)
    {
        return m_sub_primitive_binding[id].m_is_index_16bit;
    }

    int VulkanPrimitiveBinding::GetVertexOffset()
    {
        return m_vertex_offset;
    }

}