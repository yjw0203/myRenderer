#include "yjw_vulkan_attachments_set.h"
#include "yjw_vulkan_resource_view_manager.h"
#include "yjw_vulkan_pipeline.h"
#include "yjw_vulkan_god_objects_manager.h"
#include <stdexcept>
namespace rhi
{
    std::shared_ptr<RHIAttachmentsSetLocation> VulkanAttachmentsSet::createAttachmentsSet(std::shared_ptr<RHIAttachmentsSet> attachmentsSet)
    {
        std::shared_ptr<VulkanAttachmentsSetLocation> location = std::make_shared<VulkanAttachmentsSetLocation>();

        std::vector<VkImageView> attachments(attachmentsSet->color_attachments.size() + (attachmentsSet->depth_stencil_attachments.get() != nullptr));
        for (auto attachment : attachmentsSet->color_attachments)
        {
            attachments[attachment.first] = *(((VulkanResourceViewLocation*)attachment.second->resourceView->resourceViewLocation)->getVkImageView());
        }
        if (attachmentsSet->depth_stencil_attachments.get() != nullptr)
        {
            attachments.back() = *(((VulkanResourceViewLocation*)(attachmentsSet->depth_stencil_attachments->resourceView->resourceViewLocation))->getVkImageView());
        }
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = ((VulkanPipelineLocation*)attachmentsSet->pipeline->location.get())->renderPass;
        framebufferInfo.attachmentCount = attachments.size();
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = attachmentsSet->width;
        framebufferInfo.height = attachmentsSet->height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(vulkanGod.device, &framebufferInfo, nullptr, &location->frameBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
        return location;
    }

    void VulkanAttachmentsSet::destoryAttachmentsSet(std::shared_ptr<RHIAttachmentsSetLocation> location)
    {
        vkDestroyFramebuffer(vulkanGod.device, ((VulkanAttachmentsSetLocation*)location.get())->frameBuffer, nullptr);
    }
}