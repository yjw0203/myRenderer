#pragma once
#include "rhi/new_rhi/yjw_rhi_resource.h"
#include "rhi/vulkan/resource/yjw_vulkan_resource_header.h"
namespace vulkan
{
    class ResourceCreationAdaptor
    {
    public:
        ResourceCreationAdaptor(const rhi::RHIResourceCreation& initConfig) : payload(initConfig){}
        operator VulkanTextureCreation();
        operator VulkanBufferCreation();
    private:
        const rhi::RHIResourceCreation& payload;
    };
}