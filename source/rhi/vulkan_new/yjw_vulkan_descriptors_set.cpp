#include "yjw_vulkan_descriptors_set.h"
#include "yjw_vulkan_god_objects_manager.h"
#include "yjw_vulkan_pipeline.h"
#include "yjw_vulkan_resource_manager.h"
#include "yjw_vulkan_resource_view_manager.h"
#include <array>
#include <stdexcept>
namespace rhi
{
    std::shared_ptr<RHIDescriptorsSetLocation> VulkanDescritporsSet::createDescriptorsSet(std::shared_ptr<RHIDescriptorsSet> descriptorsSet)
    {
        std::shared_ptr<VulkanDescriptorsSetLocation> location = std::make_shared<VulkanDescriptorsSetLocation>();

        int count_sampler2D = 0;
        int count_unformBuffer = 0;
        for (auto descriptor : descriptorsSet->decriptors)
        {
            if (descriptor.second->type == RHIDescriptorType::uniform_buffer)
            {
                count_unformBuffer++;
            }
            else if (descriptor.second->type == RHIDescriptorType::shader_resource_texture)
            {
                count_sampler2D++;
            }
        }

        //poor
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[0].descriptorCount = count_sampler2D;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[1].descriptorCount = count_unformBuffer;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = descriptorsSet->num_set;

        if (vkCreateDescriptorPool(vulkanGod.device, &poolInfo, nullptr, &location->descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }

        //set
        location->descriptorSets.resize(descriptorsSet->num_set);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = location->descriptorPool;
        allocInfo.descriptorSetCount = location->descriptorSets.size();
        allocInfo.pSetLayouts = ((VulkanPipelineLocation*)descriptorsSet->pipeline->location.get())->descriptorSetLayouts.data();

        VkResult hr;
        if ((hr = vkAllocateDescriptorSets(vulkanGod.device, &allocInfo, location->descriptorSets.data())) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        //descriptor write
        std::vector<VkDescriptorImageInfo> imageInfos;
        imageInfos.reserve(count_sampler2D);
        std::vector<VkDescriptorBufferInfo> bufferInfos;
        bufferInfos.reserve(count_unformBuffer);
        std::vector<VkWriteDescriptorSet> descriptorWrites;
        for (auto descriptor : descriptorsSet->decriptors)
        {
            if (descriptor.second->type == RHIDescriptorType::uniform_buffer)
            {
                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = *((VulkanResourceLocation*)(((RHIUniformBuffer*)descriptor.second.get())->resource->resourceLocation))->getVkBuffer();
                bufferInfo.offset = 0;
                bufferInfo.range = ((RHIUniformBuffer*)descriptor.second.get())->size;
                bufferInfos.push_back(bufferInfo);

                VkWriteDescriptorSet write{};
                write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write.dstSet = location->descriptorSets[descriptor.first.first];
                write.dstBinding = descriptor.first.second;
                write.dstArrayElement = 0;
                write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                write.descriptorCount = 1;
                write.pBufferInfo = &bufferInfos.back();
                descriptorWrites.push_back(write);
            }
            else if (descriptor.second->type == RHIDescriptorType::shader_resource_texture)
            {
                VkDescriptorImageInfo imageInfo{};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = *((VulkanResourceViewLocation*)(((RHIShaderResourceTexture*)descriptor.second.get())->resourceView->resourceViewLocation))->getVkImageView();
                imageInfo.sampler = vulkanGod.defaultSampler;
                imageInfos.push_back(imageInfo);

                VkWriteDescriptorSet write{};
                write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write.dstSet = location->descriptorSets[descriptor.first.first];
                write.dstBinding = descriptor.first.second;
                write.dstArrayElement = 0;
                write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                write.descriptorCount = 1;
                write.pImageInfo = &imageInfos.back();
                descriptorWrites.push_back(write);
            }
        }

        vkUpdateDescriptorSets(vulkanGod.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

        return location;
    }

    void VulkanDescritporsSet::destoryDescriptorsSet(std::shared_ptr<RHIDescriptorsSetLocation> location)
    {
        vkDestroyDescriptorPool(vulkanGod.device, ((VulkanDescriptorsSetLocation*)location.get())->descriptorPool, nullptr);
    }
}