#include "yjw_vulkan.h"
#include "vulkan/vulkan.h"
#include "yjw_vulkan_descriptor_set.h"
#include "rhi/vulkan/resource/yjw_vulkan_descriptor.h"
#include "yjw_vulkan_resource_ruler.h"

namespace vulkan
{
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
            else if (descriptor->descriptor_type == VulkanDescriptorType::shader_resource && descriptor->resource_type == VulkanResourceType::texture)
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
            else if (descriptor->descriptor_type == VulkanDescriptorType::shader_resource && descriptor->resource_type == VulkanResourceType::texture)
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
}