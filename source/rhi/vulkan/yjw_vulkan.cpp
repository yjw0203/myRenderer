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

namespace vulkan
{
    rhi::RHIResourceHandle VulkanRHI::createResource(const rhi::RHIResourceCreation& creation)
    {
        if (creation.type == rhi::ResourceType::buffer)
        {
            return VK_G(BufferPool).allocateBuffer(ResourceCreationAdaptor(creation));
        }
        else
        {
            return VK_G(TexturePool).createTexture(ResourceCreationAdaptor(creation));
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

    rhi::RHIPSOHandle VulkanRHI::createPSO(const rhi::PSOCreation& creation)
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
}