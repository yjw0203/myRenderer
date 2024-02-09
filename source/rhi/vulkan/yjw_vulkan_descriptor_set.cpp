#include "yjw_vulkan_descriptor_set.h"
#include <array>
#include <stdexcept>
namespace vulkan
{
	VulkanDescriptorSet* DefaultVulkanDescriptorSetAllocateStrategy::CreateFunc(VulkanDescariptorSetCreation& creation)
	{
		VulkanDescriptorSet* vulkanDescriptorSet = new VulkanDescriptorSet();

        VulkanPSO* pso = creation.pso.Get();
        PSODescriptorLayoutBinding& descriptorSetLayout = pso->descriptorLayoutBinding;

		int count_sampler2D = descriptorSetLayout.queryCount(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
		int count_unformBuffer = descriptorSetLayout.queryCount(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		int num_set = descriptorSetLayout.bindings.size();

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
		poolInfo.maxSets = num_set;

		if (vkCreateDescriptorPool(VK_G(VkDevice), &poolInfo, nullptr, &vulkanDescriptorSet->descriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}

		vulkanDescriptorSet->descriptorSets.resize(num_set);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = vulkanDescriptorSet->descriptorPool;
        allocInfo.descriptorSetCount = num_set;
        allocInfo.pSetLayouts = pso->descriptorSetLayouts.data();

        VkResult hr;
        if ((hr = vkAllocateDescriptorSets(VK_G(VkDevice), &allocInfo, vulkanDescriptorSet->descriptorSets.data())) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        /* todo: write default resource
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

        vkUpdateDescriptorSets(vulkanGod.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);*/
	}

	void DefaultVulkanDescriptorSetAllocateStrategy::DestoryFunc(VulkanDescriptorSet* descriptorSet)
	{
        vkFreeDescriptorSets(VK_G(VkDevice), descriptorSet->descriptorPool, descriptorSet->descriptorSets.size(), descriptorSet->descriptorSets.data());
        vkDestroyDescriptorPool(VK_G(VkDevice), descriptorSet->descriptorPool, nullptr);
	}

	VulkanDescriptorSetHandle VulkanDescriptorSetPool::createDescriptorSet(VulkanDescariptorSetCreation& creation)
	{
		return DefaultAllocator.create(creation);
	}

	void VulkanDescriptorSetPool::destoryDescriptorSet(VulkanDescriptorSetHandle handle)
	{
		DefaultAllocator.destory(handle);
	}
}