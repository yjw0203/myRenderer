#include "yjw_vulkan.h"
#include "rhi/vulkan/resource/yjw_vulkan_resource_header.h"
#include "rhi/vulkan/shader/yjw_shader_header.h"
#include "rhi/vulkan/adaptor/yjw_resource_adaptor.h"
#include "rhi/vulkan/adaptor/yjw_shader_adaptor.h"
#include "rhi/vulkan/adaptor/yjw_pso_adaptor.h"
#include "rhi/vulkan/adaptor/yjw_command_adaptor.h"
#include "command/yjw_vulkan_command_buffer.h"
#include "rhi/vulkan/yjw_vulkan_resource_ruler.h"

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
        if (resource_type(resource) == VulkanResourceType::buffer)
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
}