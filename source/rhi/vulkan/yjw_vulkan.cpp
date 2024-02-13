#include "yjw_vulkan.h"
#include "rhi/vulkan/resource/yjw_vulkan_resource_header.h"
#include "rhi/vulkan/shader/yjw_shader_header.h"
#include "rhi/vulkan/adaptor/yjw_resource_adaptor.h"
#include "rhi/vulkan/adaptor/yjw_shader_adaptor.h"
#include "rhi/vulkan/adaptor/yjw_pso_adaptor.h"
#include "rhi/vulkan/adaptor/yjw_command_adaptor.h"
#include "command/yjw_vulkan_command_buffer.h"
#include "rhi/vulkan/yjw_vulkan_resource_ruler.h"
#include "rhi/vulkan/yjw_vulkan_descriptor_set.h"
#include "rhi/vulkan/resource/yjw_vulkan_descriptor.h"
#include "rhi/vulkan/adaptor/yjw_descriptor_adaptor.h"
#include "rhi/vulkan/yjw_vulkan_attachment_set.h"
#include "rhi/vulkan/adaptor/yjw_property_adaptor.h"
#include <stdexcept>

namespace vulkan
{
    rhi::RHIResourceHandle VulkanRHI::createResource(const rhi::RHIResourceCreation& creation)
    {
        if (creation.type == rhi::ResourceType::buffer)
        {
            VulkanBufferCreation vkcreation = ResourceCreationAdaptor(creation);
            return VK_G(BufferPool).allocateBuffer(vkcreation);
        }
        else
        {
            VulkanTextureCreation vkcreation = ResourceCreationAdaptor(creation);
            return VK_G(TexturePool).createTexture(vkcreation);
        }
    }

    void VulkanRHI::destoryResource(rhi::RHIResourceHandle resource)
    {
        if (get_resource_type(resource) == VulkanResourceType::buffer)
        {
            VK_G(BufferPool).deallocateBuffer(resource);
        }
        else
        {
            VK_G(TexturePool).destroyTexture(resource);
        }
    }

    rhi::RHIResourceState VulkanRHI::getResourceState(rhi::RHIResourceHandle resource)
    {
        if (get_resource_type(resource) == VulkanResourceType::texture)
        {
            VulkanTexture* texture = HandleCast<VulkanTexture>(resource);
            return ResouraceStateAdptor(texture->currentState);
        }
        return rhi::RHIResourceState::undefine;
    }

    rhi::RHIPSOHandle VulkanRHI::createPSO(rhi::PSOCreation& creation)
    {
        PSOCreationAdaptor vkcreation(creation);
        return VK_G(VulkanPSOPool).createPSO(vkcreation);
    }

    void VulkanRHI::destoryPSO(rhi::RHIPSOHandle handle)
    {
        VK_G(VulkanPSOPool).destroyPSO(handle);
    }

    rhi::RHIShaderHandle VulkanRHI::createShader(rhi::RHIShaderCreation& creation)
    {
        VulkanShaderCreationAdaptor vkShader(creation);
        return VK_G(ShaderPool).createShader(vkShader);
    }

    void VulkanRHI::destoryShader(rhi::RHIShaderHandle shader)
    {
        VK_G(ShaderPool).destoryShader(shader);
    }

    rhi::RHICommandBufferHandle VulkanRHI::createCommandBuffer(const rhi::RHICommandBufferCreation& creation)
    {
        return VK_G(CommandBufferPool).allocateCommandBuffer(RHICommandBufferCreationAdaptor(creation));
    }

    void VulkanRHI::destoryCommandBuffer(rhi::RHICommandBufferHandle handle)
    {
        VK_G(CommandBufferPool).destoryCommandBuffer(handle);
    }

    rhi::RHIDescriptorSetHandle VulkanRHI::createDescriptorSet(rhi::RHIPSOHandle pso)
    {
        VulkanDescariptorSetCreation creation;
        creation.pso = pso;
        return VK_G(VulkanDescriptorSetPool).createDescriptorSet(creation);
    }

    void VulkanRHI::destoryDescriptorSet(rhi::RHIDescriptorSetHandle descriptorSet)
    {
        VK_G(VulkanDescriptorSetPool).destoryDescriptorSet(descriptorSet);
    }

    rhi::RHIDescriptorHandle VulkanRHI::createDescriptor(rhi::RHIDescriptorCreation creation)
    {
        VulkanDescriptorCreation vkcreation = VulkanDescriptorCreationAdaptor(creation);
        return VK_G(VulkanDescriptorPool).create(vkcreation);
    }

    void VulkanRHI::destoryDescriptor(rhi::RHIDescriptorHandle descriptor)
    {
        VK_G(VulkanDescriptorPool).destory(descriptor);
    }

    rhi::RHIAttachmentSetHandle VulkanRHI::createAttachmentSet(rhi::RHIAttachmentSetCreation creation)
    {
        VulkanAttachmentSetCreationAdaptor vkcreation = VulkanAttachmentSetCreationAdaptor(creation);
        return VK_G(VulkanAttachmentSetPool).createAttachmentSet(vkcreation);
    }

    void VulkanRHI::destoryAttachmentSet(rhi::RHIAttachmentSetHandle attachmentSetHandle)
    {
        VK_G(VulkanAttachmentSetPool).destoryAttachmentSet(attachmentSetHandle);
    }
    void VulkanRHI::submitCommandBuffer(rhi::RHICommandBufferHandle commandBufferHandle)
    {
        VulkanCommandBuffer* commandBuffer = HandleCast<VulkanCommandBuffer>(commandBufferHandle);
        vkEndCommandBuffer(commandBuffer->commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { VK_G(VulkanDefaultResource).imageAvailableSemaphore };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 0;
        submitInfo.pWaitSemaphores = nullptr;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer->commandBuffer;

        submitInfo.signalSemaphoreCount = 0;
        submitInfo.pSignalSemaphores = nullptr;

        VkResult vr;
        if ((vr = vkQueueSubmit(VK_G(VkGraphicsQueue), 1, &submitInfo, commandBuffer->fence)) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }
    }
    void VulkanRHI::resetCommandBuffer(rhi::RHICommandBufferHandle commandBufferHandle)
    {
        VulkanCommandBuffer* commandBuffer = HandleCast<VulkanCommandBuffer>(commandBufferHandle);
        vkWaitForFences(VK_G(VkDevice), 1, &commandBuffer->fence, VK_TRUE, UINT64_MAX);
        vkResetFences(VK_G(VkDevice), 1, &commandBuffer->fence);

        //reset command buffer
        vkResetCommandBuffer(commandBuffer->commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(commandBuffer->commandBuffer, &beginInfo);
    }

    void VulkanRHI::waitForIdle()
    {
        vkQueueWaitIdle(VK_G(VkGraphicsQueue));
    }

    void VulkanRHI::present()
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 0;;
        presentInfo.pWaitSemaphores = nullptr;

        VkSwapchainKHR swapChains[] = { VK_G(VkSwapchainKHR) };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &VK_G(SwapChainInfo).swapchainImageIndex;

        vkQueuePresentKHR(VK_G(VkPresentQueue), &presentInfo);
        vkQueueWaitIdle(VK_G(VkGraphicsQueue));
        vkAcquireNextImageKHR(VK_G(VkDevice), VK_G(VkSwapchainKHR), UINT64_MAX, VK_G(VulkanDefaultResource).imageAvailableSemaphore, VK_NULL_HANDLE, &VK_G(SwapChainInfo).swapchainImageIndex);
    }
}