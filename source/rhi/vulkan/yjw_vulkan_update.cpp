#include "yjw_vulkan.h"
#include "vulkan/vulkan.h"
#include "yjw_vulkan_descriptor_set.h"
#include "rhi/vulkan/resource/yjw_vulkan_descriptor.h"
#include "yjw_vulkan_resource_ruler.h"
#include <stdexcept>
#include "command/yjw_vulkan_command_buffer.h"
namespace vulkan
{
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(VK_G(VkDevice), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(VK_G(VkDevice), buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType_(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(VK_G(VkDevice), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(VK_G(VkDevice), buffer, bufferMemory, 0);
    }

    void VulkanRHI::writeDescriptorSet(rhi::RHIDescriptorSetHandle descriptorSetHandle, rhi::DescriptorWrite* writes, int count)
    {
        VulkanDescriptorSet* descriptorSet = ((VulkanDescriptorSetHandle)descriptorSetHandle).Get();
        int count_sampler2D = 0;
        int count_unformBuffer = 0;
        for (int i = 0; i < count; i++)
        {
            VulkanDescriptor* descriptor = ((VulkanDescriptorHandle)writes[i].descriptor).Get();
            if (descriptor->descriptor_type == VulkanDescriptorType::uniform_buffer)
            {
                count_unformBuffer++;
            }
            else if (descriptor->descriptor_type == VulkanDescriptorType::shader_resource_texture && descriptor->resource_type == VulkanResourceType::texture)
            {
                count_sampler2D++;
            }
        }

        //descriptor write
        std::vector<VkDescriptorImageInfo> imageInfos;
        imageInfos.reserve(count_sampler2D);
        std::vector<VkDescriptorBufferInfo> bufferInfos;
        bufferInfos.reserve(count_unformBuffer);
        std::vector<VkWriteDescriptorSet> descriptorWrites;
        descriptorWrites.reserve(count_sampler2D + count_unformBuffer);
        for (int i = 0; i < count; i++)
        {
            VulkanDescriptor* descriptor = ((VulkanDescriptorHandle)writes[i].descriptor).Get();
            if (descriptor->descriptor_type == VulkanDescriptorType::uniform_buffer)
            {
                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = *descriptor;
                bufferInfo.offset = descriptor->getBufferOffset();
                bufferInfo.range = descriptor->getBufferSize();
                bufferInfos.push_back(bufferInfo);

                VkWriteDescriptorSet write{};
                write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write.dstSet = descriptorSet->getDescriptorSet(writes[i].setId);
                write.dstBinding = writes[i].slot;
                write.dstArrayElement = 0;
                write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                write.descriptorCount = 1;
                write.pBufferInfo = &bufferInfos.back();
                descriptorWrites.push_back(write);
            }
            else if (descriptor->descriptor_type == VulkanDescriptorType::shader_resource_texture && descriptor->resource_type == VulkanResourceType::texture)
            {
                VkDescriptorImageInfo imageInfo{};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = *descriptor;
                imageInfo.sampler = VK_G(VulkanDefaultResource).DefaultSampler;
                imageInfos.push_back(imageInfo);

                VkWriteDescriptorSet write{};
                write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write.dstSet = descriptorSet->getDescriptorSet(writes[i].setId);
                write.dstBinding = writes[i].slot;
                write.dstArrayElement = 0;
                write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                write.descriptorCount = 1;
                write.pImageInfo = &imageInfos.back();
                descriptorWrites.push_back(write);
            }
        }

        vkUpdateDescriptorSets(VK_G(VkDevice), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }

    void VulkanRHI::updateResource(rhi::RHIResourceHandle resourceHandle, void* data,int offset, int size)
    {
        if (get_resource_type(resourceHandle) == VulkanResourceType::buffer)
        {
            VulkanBuffer* buffer = HandleCast<VulkanBuffer>(resourceHandle);
            if (buffer->memoryType & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
            {
                VkBuffer stagingBuffer;
                VkDeviceMemory stagingBufferMemory;
                createBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

                void* map_data;
                vkMapMemory(VK_G(VkDevice), stagingBufferMemory, 0, size, 0, &map_data);
                memcpy(map_data, data, (size_t)size);
                vkUnmapMemory(VK_G(VkDevice), stagingBufferMemory);

                VkCommandBuffer commandBuffer = VK_G(CommandBufferPool).beginImmdiatelyCommandBuffer();
                VkBufferCopy copyRegion{};
                copyRegion.dstOffset = offset;
                copyRegion.size = size;
                vkCmdCopyBuffer(commandBuffer, stagingBuffer, *buffer, 1, &copyRegion);
                VK_G(CommandBufferPool).endImmdiatelyCommandBuffer(commandBuffer);

                vkDestroyBuffer(VK_G(VkDevice), stagingBuffer, nullptr);
                vkFreeMemory(VK_G(VkDevice), stagingBufferMemory, nullptr);
            }
            else
            {
                void* buffer_map;
                vkMapMemory(VK_G(VkDevice), *buffer, offset, size, 0, &buffer_map);
                memcpy(buffer_map, data, static_cast<size_t>(size));
                vkUnmapMemory(VK_G(VkDevice), *buffer);
            }
        }
        else if (get_resource_type(resourceHandle) == VulkanResourceType::texture)
        {

        }
    }
}