#include "yjw_vulkan_rhi.h"
#include "vulkan/vulkan.h"
#include "yjw_vulkan_shader_manager.h"
#include "yjw_vulkan_resource_manager.h"
#include "yjw_vulkan_resource_view_manager.h"
#include "yjw_vulkan_god_objects_manager.h"
#include "yjw_vulkan_draw_template_manager.h"
#include "yjw_vulkan_command_buffer_manager.h" 
#include "yjw_vulkan_type_conversion.h" 

namespace rhi
{
    void IVulkanRHI::initialize(CreateInfo info)
    {
        vulkanGod.initialize(info);
    }

    void IVulkanRHI::beginFrame()
    {
        vulkanGod.beginFrame();
    }

    void IVulkanRHI::endFrame(RHIResource* present_texture)
    {
        vulkanGod.endFrame(present_texture);
    }

    void IVulkanRHI::resourceBarrier(RHIResource* resource, RHIResourceState beforeState, RHIResourceState afterState)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().beginOneTimeCommandBuffer();
        transitionImageLayout(commandBuffer, *((VulkanResourceLocation*)resource->resourceLocation)->getVkImage(), ((VulkanResourceLocation*)resource->resourceLocation)->getDesc().imageInfo.format, VulkanConverter::convertResourceState(beforeState), VulkanConverter::convertResourceState(afterState));
        VulkanCommandBufferAllocater::Get().endOneTimeCommandBuffer(commandBuffer);
    }

    void IVulkanRHI::resourceBarrierImmidiately(RHIResource* resource, RHIResourceState beforeState, RHIResourceState afterState)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().beginImmdiatelyCommandBuffer();
        transitionImageLayout(commandBuffer, *((VulkanResourceLocation*)resource->resourceLocation)->getVkImage(), ((VulkanResourceLocation*)resource->resourceLocation)->getDesc().imageInfo.format, VulkanConverter::convertResourceState(beforeState), VulkanConverter::convertResourceState(afterState));
        VulkanCommandBufferAllocater::Get().endImmdiatelyCommandBuffer(commandBuffer);
    }

    void IVulkanRHI::copyResourceImmidiately(RHIResource* src, RHIResource* dst)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().beginImmdiatelyCommandBuffer();
        VulkanResourceCopyer::copyResource(commandBuffer, src, dst);
        VulkanCommandBufferAllocater::Get().endImmdiatelyCommandBuffer(commandBuffer);
    }

    void IVulkanRHI::writeResourceImmidiately(RHIResource* resource, void* data, int size)
    {
        VulkanResourceWriter::writeResourceImmidiately(resource, data, size);
    }

    void IVulkanRHI::clearImageResource(RHIResource* resource)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().beginOneTimeCommandBuffer();
        if (resource->getDesc().format == D24_unorm_S8_uint)
        {
            VkClearDepthStencilValue value{};
            value.depth = 1.0f;
            value.stencil = 0;
            VkImageSubresourceRange range{};
            range.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
            range.baseArrayLayer = 0;
            range.baseMipLevel = 0;
            range.layerCount = 1;
            range.levelCount = 1;
            vkCmdClearDepthStencilImage(commandBuffer, *((VulkanResourceLocation*)resource->resourceLocation)->getVkImage(), VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value, 1, &range);
        }
        else
        {
            VkClearColorValue value{ { 0,0,0,0 } };
            VkImageSubresourceRange range{};
            range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            range.baseArrayLayer = 0;
            range.baseMipLevel = 0;
            range.layerCount = 1;
            range.levelCount = 1;
            vkCmdClearColorImage(commandBuffer, *((VulkanResourceLocation*)resource->resourceLocation)->getVkImage(), VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value, 1, &range);
        }
        VulkanCommandBufferAllocater::Get().endOneTimeCommandBuffer(commandBuffer);
    }

    RHIResourceLocation* IVulkanRHI::createResource(const RHIResourceDesc& rhi_desc)
    {
        return VulkanResourceManager::Get().createResource(rhi_desc);
    }

    void IVulkanRHI::deleteResource(RHIResourceLocation*& location)
    {
        VulkanResourceManager::Get().deleteResource(location);
    }

    RHIResourceViewLocation* IVulkanRHI::createResourceView(ResourceViewType type, RHIResource* resource, RHIFormat view_format)
    {
        return VulkanResourceViewManager::Get().createResourceView(type, resource, view_format);
    }

    void IVulkanRHI::deleteResourceView(RHIResourceViewLocation*& location)
    {
        VulkanResourceViewManager::Get().deleteResourceView(location);
    }

    RHIShaderLocation* IVulkanRHI::createShader(const RHIShaderDesc& rhi_desc)
    {
        return VulkanShaderManager::Get().createShader(rhi_desc);
    }

    void IVulkanRHI::deleteShader(RHIShaderLocation*& location)
    {
        VulkanShaderManager::Get().deleteShader(location);
    }

    void IVulkanRHI::excuteDrawTemplate(const DefaultDrawTemplate* draw_template)
    {
        VulkanDefaultDrawTemplateBuilder::excuteDrawTemplate(draw_template);
    }

    DefaultDrawTemplateBuildCache* IVulkanRHI::buildDrawTemplateCache(DefaultDrawTemplate* draw_template)
    {
        return VulkanDefaultDrawTemplateBuilder::buildDrawTemplateCache(draw_template);
    }

    void IVulkanRHI::deleteDrawTemplateCache(DefaultDrawTemplateBuildCache*& draw_template_cache)
    {
        VulkanDefaultDrawTemplateBuilder::deleteDrawTemplateCache(draw_template_cache);
    }
}