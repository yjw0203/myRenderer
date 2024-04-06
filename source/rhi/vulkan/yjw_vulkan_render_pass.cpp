#include "yjw_vulkan_render_pass.h"
#include "yjw_vulkan_resource_cast.h"
#include "yjw_vulkan_texture.h"
#include <vector>
#include "vulkan/vulkan.h"

namespace rhi
{
    VulkanRenderPass::VulkanRenderPass(VulkanDevice* device, const RHIRenderPassDescriptor& desc)
        : VulkanDeviceObject(device)
        , RHIRenderPass(desc)
    {
        std::vector<VkAttachmentDescription> renderPass_attachments(desc.colorAttachmentCount + (desc.depthStencilAttachment != nullptr));
        std::vector<VkAttachmentReference> renderPass_attachmentRefs(desc.colorAttachmentCount + (desc.depthStencilAttachment != nullptr));
        VkSubpassDescription renderPass_subpass{};
        VkSubpassDependency renderPass_dependency{};
        for (int index = 0; index < desc.colorAttachmentCount; index++)
        {
            renderPass_attachments[index].format = ResourceCast(desc.colorAttachments[index])->GetVkFormat();
            renderPass_attachments[index].samples = VK_SAMPLE_COUNT_1_BIT;
            renderPass_attachments[index].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            renderPass_attachments[index].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            renderPass_attachments[index].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            renderPass_attachments[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            renderPass_attachments[index].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            renderPass_attachments[index].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            m_width = ResourceCast(desc.colorAttachments[index])->GetWidth();
            m_height = ResourceCast(desc.colorAttachments[index])->GetHeight();
        }

        if (desc.depthStencilAttachment)
        {
            renderPass_attachments.back().format = ResourceCast(desc.depthStencilAttachment)->GetVkFormat();
            renderPass_attachments.back().samples = VK_SAMPLE_COUNT_1_BIT;
            renderPass_attachments.back().loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            renderPass_attachments.back().storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            renderPass_attachments.back().stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            renderPass_attachments.back().stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            renderPass_attachments.back().initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            renderPass_attachments.back().finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            m_width = ResourceCast(desc.depthStencilAttachment)->GetWidth();
            m_height = ResourceCast(desc.depthStencilAttachment)->GetHeight();
        }

        for (int index = 0; index < desc.colorAttachmentCount; index++)
        {
            renderPass_attachmentRefs[index].attachment = index;
            renderPass_attachmentRefs[index].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
        if (desc.depthStencilAttachment)
        {
            renderPass_attachmentRefs.back().attachment = (int)renderPass_attachmentRefs.size() - 1;
            renderPass_attachmentRefs.back().layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        }

        renderPass_subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        renderPass_subpass.colorAttachmentCount = desc.colorAttachmentCount;
        renderPass_subpass.pColorAttachments = renderPass_attachmentRefs.data();
        if (desc.depthStencilAttachment)
        {
            renderPass_subpass.pDepthStencilAttachment = &renderPass_attachmentRefs.back();
        }

        renderPass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        renderPass_dependency.dstSubpass = 0;
        renderPass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        renderPass_dependency.srcAccessMask = 0;
        renderPass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        renderPass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.attachmentCount = renderPass_attachments.size();
        createInfo.pAttachments = renderPass_attachments.data();
        createInfo.subpassCount = 1;
        createInfo.pSubpasses = &renderPass_subpass;
        createInfo.dependencyCount = 1;
        createInfo.pDependencies = &renderPass_dependency;
        vkCreateRenderPass(device->GetNativeDevice(), &createInfo, nullptr, &m_native_render_pass);

        std::vector<VkImageView> imageViews(desc.colorAttachmentCount + (desc.depthStencilAttachment != nullptr));
        for (int index = 0; index < desc.colorAttachmentCount; index++)
        {
            imageViews[index] = ResourceCast(desc.colorAttachments[index])->GetVkImageView();
        }
        if (desc.depthStencilAttachment)
        {
            imageViews.back() = ResourceCast(desc.depthStencilAttachment)->GetVkImageView();
        }

        VkFramebufferCreateInfo framebufferCreateInfo{};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.flags = 0;
        framebufferCreateInfo.pNext = nullptr;
        framebufferCreateInfo.renderPass = m_native_render_pass;
        framebufferCreateInfo.width = m_width;
        framebufferCreateInfo.height = m_height;
        framebufferCreateInfo.layers = 1;
        framebufferCreateInfo.attachmentCount = imageViews.size();
        framebufferCreateInfo.pAttachments = imageViews.data();
        vkCreateFramebuffer(device->GetNativeDevice(), &framebufferCreateInfo, nullptr, &m_frame_buffer);

        for (int index = 0; index < desc.colorAttachmentCount; index++)
        {
            ResourceCast(desc.colorAttachments[index])->retain(this);
        }
        if (desc.depthStencilAttachment)
        {
            ResourceCast(desc.depthStencilAttachment)->retain(this);
        }
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        if (m_frame_buffer)
        {
            vkDestroyFramebuffer(GetDevice()->GetNativeDevice(), m_frame_buffer, nullptr);
        }

        if (m_native_render_pass)
        {
            vkDestroyRenderPass(GetDevice()->GetNativeDevice(), m_native_render_pass, nullptr);
        }

        for (int index = 0; index < GetDesc().colorAttachmentCount; index++)
        {
            ResourceCast(GetDesc().colorAttachments[index])->release();
        }
        if (GetDesc().depthStencilAttachment)
        {
            ResourceCast(GetDesc().depthStencilAttachment)->release();
        }
    }

    VkRenderPass VulkanRenderPass::GetNativeRenderPass()
    {
        return m_native_render_pass;
    }

    VkFramebuffer VulkanRenderPass::GetFramebuffer()
    {
        return m_frame_buffer;
    }

    int VulkanRenderPass::GetWidth()
    {
        return m_width;
    }

    int VulkanRenderPass::GetHeight()
    {
        return m_height;
    }

    void VulkanRenderPass::TransitionStateToRender(VkCommandBuffer commandBuffer)
    {
        for (int index = 0; index < GetDesc().colorAttachmentCount; index++)
        {
            ResourceCast(GetDesc().colorAttachments[index])->GetTexture()->TransitionState(commandBuffer, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        }
        if (GetDesc().depthStencilAttachment)
        {
            ResourceCast(GetDesc().depthStencilAttachment)->GetTexture()->TransitionState(commandBuffer, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        }
    }
}