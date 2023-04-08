#pragma once

#include "rhi/rhi/yjw_rhi_resource.h"
#include "vulkan/vulkan.h"
namespace rhi
{
    typedef struct VulkanResourceDesc
    {
        RHIResourceType type;
        union {
            VkBufferCreateInfo bufferInfo;
            VkImageCreateInfo imageInfo;
        };
    }VulkanResourceDesc;

    class VulkanResourceLocation : public RHIResourceLocation
    {
        friend class VulkanResourceManager;
    public:
        const VulkanResourceDesc& getDesc();
        const VkImage* getVkImage();
        const VkDeviceMemory* getVkDeviceMemory();
    private:
        VkImage image;
        VkDeviceMemory memory;
        VulkanResourceDesc desc;
    };

    class VulkanResourceManager
    {
    public:
        static VulkanResourceManager& Get()
        {
            static VulkanResourceManager instance;
            return instance;
        }

        RHIResourceLocation* createResource(const RHIResourceDesc& desc);
        void deleteResource(RHIResourceLocation*& location);
    };
}