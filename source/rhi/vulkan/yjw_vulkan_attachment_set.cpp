#include "yjw_vulkan_attachment_set.h"
#include <array>
#include <stdexcept>
#include <cassert>
namespace vulkan
{
	VulkanAttachmentSet* DefaultVulkanVulkanAttachmentSetAllocateStrategy::CreateFunc(VulkanAttachmentSetCreation& creation)
	{
        VulkanAttachmentSet* attachmentSet = new VulkanAttachmentSet();

        std::vector<VkImageView> attachments(creation.descriptors.size());
        int current_index = 0;
        int width = 0;
        int height = 0;
        for (VulkanDescriptorHandle& handle : creation.descriptors)
        {
            VulkanDescriptor* descriptor = handle.Get();
            assert(descriptor->resource_type == VulkanResourceType::texture);
            assert(descriptor->descriptor_type == VulkanDescriptorType::render_target || descriptor->descriptor_type == VulkanDescriptorType::depth_stencil);
            attachments[current_index] = *descriptor;
            width = descriptor->getImageWidth();
            height = descriptor->getImageHeight();
        }
        assert(width * height > 0);
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = creation.psoHandle.Get()->renderPass;
        framebufferInfo.attachmentCount = attachments.size();
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = width;
        framebufferInfo.height = height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(VK_G(VkDevice), &framebufferInfo, nullptr, &attachmentSet->frameBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
	}

	void DefaultVulkanVulkanAttachmentSetAllocateStrategy::DestoryFunc(VulkanAttachmentSet* attachmentSet)
	{
        vkDestroyFramebuffer(VK_G(VkDevice), attachmentSet->frameBuffer, nullptr);
        delete attachmentSet;
	}

	VulkanAttachmentSetHandle VulkanAttachmentSetPool::createAttachmentSet(VulkanAttachmentSetCreation creation)
	{
		return DefaultAllocator.create(creation);
	}

	void VulkanAttachmentSetPool::destoryAttachmentSet(VulkanAttachmentSetHandle handle)
	{
		DefaultAllocator.destory(handle);
	}
}