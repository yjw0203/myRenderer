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

    void IVulkanRHI::resourceBarrier(RHIResourceView* view, RHIResourceState beforeState, RHIResourceState afterState)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().beginOneTimeCommandBuffer();
        transitionImageLayout(commandBuffer, *((VulkanResourceLocation*)view->resource->resourceLocation)->getVkImage(), VulkanConverter::convertResourceState(beforeState), VulkanConverter::convertResourceState(afterState));
        VulkanCommandBufferAllocater::Get().endOneTimeCommandBuffer(commandBuffer);
    }

    void IVulkanRHI::resourceBarrierImmidiately(RHIResourceView* view, RHIResourceState beforeState, RHIResourceState afterState)
    {
        VkCommandBuffer commandBuffer = VulkanCommandBufferAllocater::Get().beginImmdiatelyCommandBuffer();
        transitionImageLayout(commandBuffer, *((VulkanResourceLocation*)view->resource->resourceLocation)->getVkImage(), VulkanConverter::convertResourceState(beforeState), VulkanConverter::convertResourceState(afterState));
        VulkanCommandBufferAllocater::Get().endImmdiatelyCommandBuffer(commandBuffer);
    }

    RHIResourceLocation* IVulkanRHI::createResource(const RHIResourceDesc& rhi_desc)
    {
        return VulkanResourceManager::Get().createResource(rhi_desc);
    }

    void IVulkanRHI::deleteResource(RHIResourceLocation*& location)
    {
        VulkanResourceManager::Get().deleteResource(location);
    }

    RHIResourceViewLocation* IVulkanRHI::createResourceView(ResourceViewType type, const RHIResource* resource, RHIFormat view_format)
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