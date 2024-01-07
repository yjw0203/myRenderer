#include "yjw_vulkan.h"
#include "rhi/vulkan/resource/yjw_vulkan_resource_header.h"
#include "rhi/vulkan/adaptor/yjw_resource_adaptor.h"

namespace vulkan
{
    void VulkanRHI::createResource(const rhi::ResourceInitConfig& initConfig, rhi::Resource* resource)
    {
        if (initConfig.type == rhi::ResourceType::buffer)
        {
            Buffer* buffer = nullptr;
            VK_G(BufferPool).allocateBuffer(ResourceInitConfigAdaptor(initConfig), buffer);
            resource = new rhi::Resource(initConfig, buffer);
        }
        else
        {
            Texture* texture = nullptr;
            VK_G(TexturePool).createTexture(ResourceInitConfigAdaptor(initConfig), texture);
            resource = new rhi::Resource(initConfig, texture);
        }
    }

    void VulkanRHI::destoryResource(rhi::Resource* resource)
    {
        if (resource->initConfig.type == rhi::ResourceType::buffer)
        {
            Buffer& buffer = BufferAdaptor(*resource);
            VK_G(BufferPool).deallocateBuffer(&buffer);
            delete resource;
        }
        else
        {
            Texture& texture = TextureAdaptor(*resource);
            VK_G(TexturePool).destroyTexture(&texture);
            delete resource;
        }
    }
}